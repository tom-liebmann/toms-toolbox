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
    std::shared_ptr< TCPSocket > TCPSocket::create()
    {
        return std::make_shared< posix::TCPSocket >();
    }


    namespace posix
    {
        TCPSocket::TCPSocket()
            : m_handle( -1 )
            , m_connectionState( ConnectionState::DISCONNECTED )
            , m_dataReader( *this )
            , m_writeOffset( 0 )
        {
        }

        TCPSocket::TCPSocket( int handle )
            : m_handle( handle )
            , m_connectionState( ConnectionState::CONNECTED )
            , m_dataReader( *this )
            , m_writeOffset( 0 )
        {
            fcntl( m_handle, F_SETFL, O_NONBLOCK );
        }

        TCPSocket::~TCPSocket()
        {
            disconnect();
        }

        TCPSocket::ConnectionState TCPSocket::connected() const
        {
            std::lock_guard< std::mutex > lock( m_mutex );
            return m_connectionState;
        }

        void TCPSocket::connect( std::string const& address, uint16_t port )
        {
            std::unique_lock< std::mutex > lock( m_mutex );

            while( m_connectionState != ConnectionState::DISCONNECTED )
            {
                lock.unlock();
                disconnect();
                lock.lock();
            }

            if( ( m_handle = ::socket( PF_INET, SOCK_STREAM, IPPROTO_TCP ) ) == -1 )
            {
                lock.unlock();

                ttb::events::ConnectionFailed event;
                eventOutput().push( event );

                return;
            }

            fcntl( m_handle, F_SETFL, O_NONBLOCK );

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

                ttb::events::ConnectionFailed event;
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

                    ttb::events::ConnectionFailed event;
                    eventOutput().push( event );

                    return;
                }
            }

            m_connectionState = ConnectionState::CONNECTING;

            auto sel = selector();
            if( sel )
                sel->notifyChange();
        }

        void TCPSocket::disconnect()
        {
            std::unique_lock< std::mutex > lock( m_mutex );

            switch( m_connectionState )
            {
                case ConnectionState::DISCONNECTED:
                    // We are already disconnected. Nothing to do here
                    break;

                case ConnectionState::CONNECTING:
                {
                    shutdown( m_handle, SHUT_RDWR );
                    close( m_handle );
                    m_handle = -1;
                    m_connectionState = ConnectionState::DISCONNECTED;
                    m_writeBuffer = std::queue< std::vector< uint8_t > >();
                    lock.unlock();

                    ttb::events::ConnectionFailed event;
                    eventOutput().push( event );
                    break;
                }

                case ConnectionState::CONNECTED:
                {
                    shutdown( m_handle, SHUT_RDWR );
                    close( m_handle );
                    m_handle = -1;
                    m_connectionState = ConnectionState::DISCONNECTED;
                    m_writeBuffer = std::queue< std::vector< uint8_t > >();
                    lock.unlock();

                    // TODO: Signal a broken disconnect
                    ttb::events::Disconnect event( ttb::events::Disconnect::Reason::NORMAL );
                    eventOutput().push( event );
                    break;
                }
            }
        }

        int TCPSocket::handle() const
        {
            std::unique_lock< std::mutex > lock( m_mutex );
            return m_handle;
        }

        bool TCPSocket::checkRead() const
        {
            std::unique_lock< std::mutex > lock( m_mutex );
            return m_connectionState != ConnectionState::DISCONNECTED;
        }

        bool TCPSocket::checkWrite() const
        {
            std::lock_guard< std::mutex > lock( m_mutex );

            // If the socket isn't connected yet, we use the writability as an indicator for
            // successful connection
            if( m_connectionState == ConnectionState::CONNECTING )
                return true;

            return false;
        }

        void TCPSocket::doRead()
        {
            std::unique_lock< std::mutex > lock( m_mutex );

            // Only perform a read if we are connected
            while( m_connectionState == ConnectionState::CONNECTED )
            {
                lock.unlock();

                if( !m_dataReader.doRead() )
                {
                    disconnect();
                    break;
                }

                bool stop = false;
                while( m_dataReader.available() )
                {
                    ttb::events::Data event( m_dataReader );
                    if( !eventOutput().push( event ) )
                    {
                        stop = true;
                        break;
                    }
                }

                if( stop )
                {
                    break;
                }

                lock.lock();
            }
        }

        void TCPSocket::doWrite()
        {
            std::unique_lock< std::mutex > lock( m_mutex );

            if( m_connectionState == ConnectionState::CONNECTING )
            {
                int result;
                socklen_t result_len = sizeof( result );
                if( getsockopt( m_handle, SOL_SOCKET, SO_ERROR, &result, &result_len ) < 0 )
                {
                    lock.unlock();
                    disconnect();
                    return;
                }

                if( result != 0 )
                {
                    lock.unlock();
                    disconnect();
                    return;
                }

                m_connectionState = ConnectionState::CONNECTED;
                lock.unlock();

                ttb::events::ServerConnection event;
                eventOutput().push( event );
            }
        }

        void TCPSocket::onData( std::vector< uint8_t > data )
        {
            std::unique_lock< std::mutex > lock( m_mutex );

            if( m_connectionState == ConnectionState::CONNECTED )
            {
                m_writeBuffer.push( std::move( data ) );

                auto sel = selector();
                if( sel )
                    sel->notifyWrite();
            }
        }

        bool TCPSocket::writeData()
        {
            std::unique_lock< std::mutex > lock( m_mutex );
            while( !m_writeBuffer.empty() )
            {
                auto data = m_writeBuffer.front();

                while( m_writeOffset < data.size() )
                {
                    if( m_connectionState != ConnectionState::CONNECTED )
                        break;

                    auto handle = m_handle;
                    lock.unlock();

                    auto result = ::send( handle,
                                          data.data() + m_writeOffset,
                                          data.size() - m_writeOffset,
                                          MSG_NOSIGNAL );

                    if( result < 0 )
                    {
                        if( errno == EAGAIN || errno == EWOULDBLOCK )
                        {
                            return false;
                        }
                        else
                        {
                            disconnect();
                            break;
                        }
                    }
                    else
                    {
                        m_writeOffset += result;
                        lock.lock();
                    }
                }

                m_writeBuffer.pop();
                m_writeOffset = 0;
            }

            return true;
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
