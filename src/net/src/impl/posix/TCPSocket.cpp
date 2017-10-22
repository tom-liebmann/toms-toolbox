#include "TCPSocket.hpp"
#include "SocketDataWriter.hpp"

#include <ttb/net/DataWriter.hpp>
#include <ttb/net/netEvents.hpp>
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

    TCPSocket::~TCPSocket() = default;


    namespace posix
    {
        TCPSocket::TCPSocket( std::string const& address, uint16_t port )
            : m_connected( false ), m_readOffset( 0 )
        {
            // create socket
            m_handle = ::socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );

            if( m_handle == -1 )
            {
                throw std::runtime_error( "Unable to create socket." );
            }

            fcntl( m_handle, F_SETFL, O_NONBLOCK );

            sockaddr_in sockAddr = createAddress( address, port );

            if(::connect( m_handle,
                          reinterpret_cast< struct sockaddr* >( &sockAddr ),
                          sizeof( sockAddr ) ) == -1 )
            {
                if( errno != EINPROGRESS )
                {
                    close( m_handle );
                    throw std::runtime_error( "Unable to connect to remote host." );
                }
            }
        }

        TCPSocket::TCPSocket( int handle ) : m_handle( handle )
        {
        }

        TCPSocket::~TCPSocket()
        {
            shutdown( m_handle, 0 );
            close( m_handle );
        }

        int TCPSocket::handle() const
        {
            return m_handle;
        }

        bool TCPSocket::isReadable() const
        {
            return true;
        }

        void TCPSocket::doRead( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput )
        {
            if( m_readOffset < sizeof( uint32_t ) )
            {
                auto result = ::recv( m_handle,
                                      reinterpret_cast< uint8_t* >( m_readBufferSize ),
                                      sizeof( uint32_t ) - m_readOffset,
                                      MSG_NOSIGNAL );

                if( result < 0 )
                {
                    if( errno != EAGAIN && errno != EWOULDBLOCK )
                    {
                        ttb::events::SocketBrokenEvent event( shared_from_this() );
                        eventOutput.push( event );
                        return;
                    }
                }

                std::cout << "Got " << result << " bytes" << std::endl;
                m_readOffset += result;

                if( m_readOffset == sizeof( uint32_t ) )
                {
                    m_readBuffer.resize( m_readBufferSize );
                }
            }

            if( m_readOffset >= sizeof( uint32_t ) )
            {
                auto result = ::recv( m_handle,
                                      m_readBuffer.data(),
                                      m_readBufferSize + sizeof( uint32_t ) - m_readOffset,
                                      MSG_NOSIGNAL );

                if( result < 0 )
                {
                    if( errno != EAGAIN && errno != EWOULDBLOCK )
                    {
                        ttb::events::SocketBrokenEvent event( shared_from_this() );
                        eventOutput.push( event );
                        return;
                    }
                }

                m_readOffset += result;

                if( m_readOffset == sizeof( uint32_t ) + m_readBufferSize )
                {
                    m_readOffset = 0;
                    ttb::events::PacketEvent event(
                        shared_from_this(),
                        std::make_shared< ttb::SizedIPacket >( std::move( m_readBuffer ) ) );
                    eventOutput.push( event );
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

        void TCPSocket::doWrite( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput )
        {
            std::lock_guard< std::mutex > lock( m_mutex );

            if( !m_connected )
            {
                m_connected = true;
                ttb::events::ConnectionEvent event( std::shared_ptr< ttb::Listener >(),
                                                    shared_from_this() );
                eventOutput.push( event );
            }

            if( !m_writeBuffer.empty() )
            {
                auto& packet = m_writeBuffer.front();

                SocketDataWriter writer( *this );

                try
                {
                    m_writeOffset +=
                        packet->write( writer, m_writeOffset, packet->size() - m_writeOffset );
                }
                catch( std::runtime_error& e )
                {
                    ttb::events::SocketBrokenEvent event( shared_from_this() );
                    eventOutput.push( event );
                    return;
                }

                if( m_writeOffset >= packet->size() )
                {
                    m_writeOffset = 0;
                    m_writeBuffer.pop();
                }
            }
        }

        void TCPSocket::send( std::shared_ptr< ttb::OPacket const > packet )
        {
            std::lock_guard< std::mutex > lock( m_mutex );

            m_writeBuffer.push( std::move( packet ) );
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
