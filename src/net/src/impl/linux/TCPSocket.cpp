#include "TCPSocket.hpp"

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
    sockaddr_in createAddress( const std::string& address, uint16_t port );
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

            sockaddr_in sockAddr = createAddress( address, port );

            if(::connect( m_handle,
                          reinterpret_cast< struct sockaddr* >( &sockAddr ),
                          sizeof( sockAddr ) ) )
            {
                close( m_handle );
                throw std::runtime_error( "Unable to connect to remote host." );
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

        void TCPSocket::send( void const* data, size_t size ) const
        {
            size_t offset = 0;
            while( offset < size )
            {
                int ret;

                while( true )
                {
                    auto ret = ::send( m_handle,
                                       reinterpret_cast< uint8_t const* >( data ) + offset,
                                       size - offset,
                                       MSG_NOSIGNAL );

                    if( ret > 0 )
                    {
                        break;
                    }
                    else if( ret < 0 )
                    {
                        if( errno != EAGAIN && errno != EWOULDBLOCK )
                        {
                            throw Error( Error::Type::BROKEN, std::string( strerror( errno ) ) );
                        }
                    }
                }

                offset += ret;
            }
        }

        void TCPSocket::receive( void* data, size_t size ) const
        {
            size_t offset = 0;

            while( offset < size )
            {
                int ret;

                while( true )
                {
                    ret = recv(
                        m_handle, reinterpret_cast< uint8_t* >( data ) + offset, size - offset, 0 );

                    if( ret > 0 )
                    {
                        break;
                    }
                    else if( ret == 0 )
                    {
                        throw Error( Error::Type::CLOSED );
                    }
                    else
                    {
                        if( errno != EAGAIN && errno != EWOULDBLOCK )
                        {
                            throw Error( Error::Type::BROKEN );
                        }
                    }
                }

                offset += ret;
            }
        }
    }
}


namespace
{
    sockaddr_in createAddress( const std::string& address, uint16_t port )
    {
        sockaddr_in sockAddr;

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
