#include "TCPSocket.hpp"

#include <ttb/net/DataWriter.hpp>

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
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


    class SocketDataWriter : public ttb::DataWriter
    {
    public:
        SocketDataWriter( ttb::linux::TCPSocket& socket );

        virtual size_t write( void const* data, size_t size ) override;

    private:
        ttb::linux::TCPSocket& m_socket;
    };
}


namespace ttb
{
    std::unique_ptr< TCPSocket > TCPSocket::connect( std::string const& address, uint16_t port )
    {
        return std::make_unique< linux::TCPSocket >( address, port );
    }

    TCPSocket::~TCPSocket() = default;


    namespace linux
    {
        TCPSocket::TCPSocket( std::string const& address, uint16_t port )
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

        int TCPSocket::handle()
        {
            return m_handle;
        }

        void TCPSocket::send( std::shared_ptr< ttb::OPacket const > packet )
        {
            std::lock_guard< std::mutex > lock( m_mutex );

            m_writeBuffer.push( std::move( packet ) );
        }

        bool TCPSocket::needsWriteUpdate() const
        {
            std::lock_guard< std::mutex > lock( m_mutex );
            return !m_writeBuffer.empty();
        }

        void TCPSocket::updateWrite()
        {
            std::lock_guard< std::mutex > lock( m_mutex );

            if( !m_writeBuffer.empty() )
            {
                auto& packet = m_writeBuffer.front();

                SocketDataWriter writer( *this );

                m_writeOffset += packet->write( writer, m_writeOffset );

                if( m_writeOffset >= packet->size() )
                {
                    m_writeOffset = 0;
                    m_writeBuffer.pop();
                }
            }
        }

        std::unique_ptr< ttb::IPacket > TCPSocket::updateRead()
        {
            return std::unique_ptr< ttb::IPacket >();
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


    SocketDataWriter::SocketDataWriter( ttb::linux::TCPSocket& socket ) : m_socket( socket )
    {
    }

    size_t SocketDataWriter::write( void const* data, size_t size )
    {
        auto result = ::send(
            m_socket.handle(), reinterpret_cast< uint8_t const* >( data ), size, MSG_NOSIGNAL );

        if( result < 0 )
        {
            if( errno != EAGAIN && errno != EWOULDBLOCK )
            {
                throw ttb::TCPSocket::Error( ttb::TCPSocket::Error::Type::BROKEN,
                                             std::string( strerror( errno ) ) );
            }

            return 0;
        }

        return result;
    }
}
