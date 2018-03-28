#include "TCPSocket.hpp"
#include "SocketDataReader.hpp"

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
    std::shared_ptr< TCPSocket > TCPSocket::connect( std::string const& address, uint16_t port )
    {
        return std::make_shared< posix::TCPSocket >( address, port );
    }


    namespace posix
    {
        TCPSocket::TCPSocket( std::string const& address, uint16_t port )
            : m_connectionState( ConnectionState::DISCONNECTED ), m_handle( -1 )
        {
            connect( address, port );
        }

        TCPSocket::TCPSocket( int handle )
            : m_connectionState( ConnectionState::CONNECTED ), m_handle( handle )
        {
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
            std::lock_guard< std::mutex > lock( m_mutex );

            if( m_connectionState != ConnectionState::DISCONNECTED )
            {
                return;
            }

            if( m_handle == -1 )
            {
                if( ( m_handle = ::socket( PF_INET, SOCK_STREAM, IPPROTO_TCP ) ) == -1 )
                {
                    // TODO: push failed connection event
                }

                fcntl( m_handle, F_SETFL, O_NONBLOCK );
            }

            auto sockAddr = createAddress( address, port );

            if(::connect( m_handle,
                          reinterpret_cast< struct sockaddr* >( &sockAddr ),
                          sizeof( sockAddr ) ) == -1 )
            {
                if( errno != EINPROGRESS )
                {
                    close( m_handle );
                    m_handle = -1;
                    // TODO: push failed connection event
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
            return m_connected;
        }

        void TCPSocket::doRead()
        {
            std::lock_guard< std::mutex > lock( m_mutex );

            if( m_connected )
            {
                SocketDataReader reader( *this );

                try
                {
                    ttb::events::Data event( reader );
                    eventOutput().push( event );
                }
                catch( std::runtime_error& e )
                {
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
            if( !m_connected )
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
            std::lock_guard< std::mutex > lock( m_mutex );

            if( !m_connected )
            {
                m_connected = true;
                ttb::events::ServerConnection event;
                eventOutput().push( event );
            }

            if( !m_writeBuffer.empty() )
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
                        ttb::events::BrokenConnection event;
                        eventOutput().push( event );
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

        void TCPSocket::onData( std::vector< uint8_t > data )
        {
            std::lock_guard< std::mutex > lock( m_mutex );

            m_writeBuffer.push( std::move( data ) );
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
