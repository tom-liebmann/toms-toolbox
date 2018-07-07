#include "TCPSocket.hpp"

#include "NetSelector.hpp"
#include <ttb/net/events.hpp>
#include <ttb/net/packets.hpp>

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


namespace
{
    sockaddr_in createAddress( std::string const& address, uint16_t port );
}


namespace ttb
{
    std::shared_ptr< TCPSocket > TCPSocket::create( std::string const& address, uint16_t port )
    {
        return std::make_shared< posix::TCPSocket >( address, port );
    }


    namespace posix
    {
        TCPSocket::TCPSocket( std::string const& address, uint16_t port )
            : m_connectionState( ConnectionState::DISCONNECTED )
            , m_writeOffset( 0 )
            , m_handle( -1 )
            , m_dataReader( *this )
        {
            connect( address, port );
        }

        TCPSocket::TCPSocket( int handle )
            : m_connectionState( ConnectionState::CONNECTED )
            , m_writeOffset( 0 )
            , m_handle( handle )
            , m_dataReader( *this )
        {
            fcntl( m_handle, F_SETFL, O_NONBLOCK );
        }

        TCPSocket::~TCPSocket()
        {
            shutdown( m_handle, SHUT_RDWR );
            close( m_handle );
        }

        TCPSocket::ConnectionState TCPSocket::connected() const
        {
            std::lock_guard< std::mutex > lock( m_mutex );
            return m_connectionState;
        }

        void TCPSocket::connect( std::string const& address, uint16_t port )
        {
            std::unique_lock< std::mutex > lock( m_mutex );

            if( m_connectionState != ConnectionState::DISCONNECTED )
            {
                return;
            }

            if( m_handle == -1 )
            {
                if( ( m_handle = ::socket( PF_INET, SOCK_STREAM, IPPROTO_TCP ) ) == -1 )
                {
                    lock.unlock();

                    ttb::events::BrokenConnection event;
                    eventOutput().push( event );

                    return;
                }

                fcntl( m_handle, F_SETFL, O_NONBLOCK );
            }

            sockaddr_in sockAddr;

            try
            {
                sockAddr = createAddress( address, port );
            }
            catch( std::runtime_error& e )
            {
                close( m_handle );
                m_handle = -1;

                lock.unlock();

                ttb::events::BrokenConnection event;
                eventOutput().push( event );

                return;
            }

            if(::connect( m_handle,
                          reinterpret_cast< struct sockaddr* >( &sockAddr ),
                          sizeof( sockAddr ) ) == -1 )
            {
                if( errno != EINPROGRESS )
                {
                    close( m_handle );
                    m_handle = -1;

                    lock.unlock();

                    ttb::events::BrokenConnection event;
                    eventOutput().push( event );

                    return;
                }
            }

            m_connectionState = ConnectionState::CONNECTING;
        }

        void TCPSocket::disconnect()
        {
            {
                std::lock_guard< std::mutex > lock( m_mutex );

                if( m_connectionState == ConnectionState::DISCONNECTED )
                {
                    return;
                }

                if( m_handle != -1 )
                {
                    shutdown( m_handle, SHUT_RDWR );
                    close( m_handle );
                    m_handle = -1;
                }

                m_connectionState = ConnectionState::DISCONNECTED;
            }

            ttb::events::Disconnect event;
            eventOutput().push( event );
        }

        int TCPSocket::handle() const
        {
            return m_handle;
        }

        bool TCPSocket::isReadable() const
        {
            return m_connectionState != ConnectionState::DISCONNECTED;
        }

        void TCPSocket::doRead()
        {
            std::unique_lock< std::mutex > lock( m_mutex );

            if( m_connectionState != ConnectionState::DISCONNECTED )
            {
                try
                {
                    lock.unlock();

                    while( true )
                    {
                        m_dataReader.doRead();

                        if( !m_dataReader.available() )
                            break;

                        ttb::events::Data event( m_dataReader );
                        eventOutput().push( event );
                    }
                }
                catch( std::runtime_error& e )
                {
                    lock.lock();

                    close( m_handle );
                    m_handle = -1;
                    m_connectionState = ConnectionState::DISCONNECTED;

                    lock.unlock();

                    ttb::events::BrokenConnection event;
                    eventOutput().push( event );
                    return;
                }
            }
        }

        bool TCPSocket::isWritable() const
        {
            std::lock_guard< std::mutex > lock( m_mutex );

            // If the socket isn't connected yet, we use the writability as an indicator for
            // successful establishment
            if( m_connectionState == ConnectionState::CONNECTING )
            {
                return true;
            }

            // There is actual data to be written
            if( !m_writeBuffer.empty() )
            {
                return true;
            }

            return false;
        }

        void TCPSocket::doWrite()
        {
            std::unique_lock< std::mutex > lock( m_mutex );

            if( m_connectionState == ConnectionState::CONNECTING )
            {
                m_connectionState = ConnectionState::CONNECTED;

                lock.unlock();

                ttb::events::ServerConnection event;
                eventOutput().push( event );

                lock.lock();
            }

            if( m_connectionState == ConnectionState::CONNECTED )
            {
                // Send out data as long as there is some available
                while( !m_writeBuffer.empty() )
                {
                    auto& buffer = m_writeBuffer.front();

                    auto result = ::send( m_handle,
                                          buffer.data() + m_writeOffset,
                                          buffer.size() - m_writeOffset,
                                          MSG_NOSIGNAL );

                    if( result < 0 )
                    {
                        if( errno != EAGAIN && errno != EWOULDBLOCK )
                        {
                            m_connectionState = ConnectionState::DISCONNECTED;
                            shutdown( m_handle, SHUT_RDWR );
                            close( m_handle );
                            m_handle = -1;
                            lock.unlock();

                            ttb::events::BrokenConnection event;
                            eventOutput().push( event );
                            return;
                        }
                        else
                        {
                            // If we got EAGAIN or EWOULDBLOCK, proceed to sending the rest on the
                            // next tick
                            return;
                        }
                    }

                    m_writeOffset += result;

                    if( m_writeOffset >= buffer.size() )
                    {
                        m_writeOffset = 0;
                        m_writeBuffer.pop();
                    }
                }
            }
        }

        void TCPSocket::selector( ttb::posix::NetSelector* selector )
        {
            std::lock_guard< std::mutex > lock( m_mutex );

            m_selector = selector;
        }

        void TCPSocket::onData( std::vector< uint8_t > data )
        {
            std::lock_guard< std::mutex > lock( m_mutex );

            m_writeBuffer.push( std::move( data ) );

            if( m_selector )
                m_selector->interrupt();
        }

        void TCPSocket::clearWriteBuffer()
        {
            std::lock_guard< std::mutex > lock( m_mutex );

            std::queue< std::vector< uint8_t > > emptyQueue;
            std::swap( m_writeBuffer, emptyQueue );
        }
    }
}


namespace
{
    sockaddr_in createAddress( std::string const& address, uint16_t port )
    {
        sockaddr_in sockAddr;

        memset( &sockAddr, 0, sizeof( sockAddr ) );

        // Address family for socket connection. Usually this is AF_INET.
        sockAddr.sin_family = AF_INET;

        // Convert port from host (h) to network (ns) byte order.
        sockAddr.sin_port = htons( port );

        // Resolve the dns to a valid address structure.
        if( inet_pton( AF_INET, address.c_str(), &sockAddr.sin_addr ) != 1 )
        {
            throw std::runtime_error( "Unable to resolve host name" );
        }

        return sockAddr;
    }
}
