#include <ttb/net/TCPSocket.hpp>

#include <ttb/net/Socket.hpp>

#include <iostream>

#ifdef WIN32
#else

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>

#endif

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
        hostent* host = gethostbyname( address.c_str() );
        if( !host )
        {
#ifdef WIN32

            auto error = WSAGetLastError();
            throw std::runtime_error( "Unable to resolve host name. (" + std::to_string( error ) +
                                      ")" );

#else

            throw std::runtime_error( "Unable to resolve host name." );

#endif
        }

// Copy first valid address to the address structure.
#ifdef WIN32

        sockAddr.sin_addr.S_un.S_addr =
            *reinterpret_cast< unsigned long* >( host->h_addr_list[ 0 ] );

#else

        sockAddr.sin_addr.s_addr = *reinterpret_cast< unsigned long* >( host->h_addr_list[ 0 ] );

#endif

        return sockAddr;
    }
}



namespace ttb
{
    TCPSocket::TCPSocket( const std::string& address, uint16_t port )
    {
#ifdef WIN32

        // Create socket.
        m_handle = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

        if( m_handle == INVALID_SOCKET )
        {
            auto error = WSAGetLastError();
            throw std::runtime_error( "Unable to create socket. (" + std::to_string( error ) +
                                      ")" );
        }

        // Connect to remote.
        auto sockAddr = createAddress( address, port );

        if( connect( m_handle, reinterpret_cast< sockaddr* >( &sockAddr ), sizeof( sockAddr ) ) )
        {
            auto error = WSAGetLastError();
            closesocket( m_handle );
            throw std::runtime_error( "Unable to connect to remote host. (" +
                                      std::to_string( error ) + ")" );
        }

#else

        // create socket
        m_handle = ::socket( AF_INET, SOCK_STREAM, 0 );

        if( m_handle == -1 )
            throw std::runtime_error( "Unable to create socket." );

        sockaddr_in sockAddr = createAddress( address, port );
        if( connect(
                m_handle, reinterpret_cast< struct sockaddr* >( &sockAddr ), sizeof( sockAddr ) ) )
        {
            close( m_handle );
            throw std::runtime_error( "Unable to connect to remote host." );
        }

#endif
    }

    TCPSocket::TCPSocket( Handle handle ) : m_handle( handle )
    {
    }

    TCPSocket::~TCPSocket()
    {
#ifdef WIN32

        shutdown( m_handle, SD_SEND );
        closesocket( m_handle );

#else

        shutdown( m_handle, 0 );
        close( m_handle );

#endif
    }

    void TCPSocket::send( const void* data, size_t size ) const
    {
        size_t offset = 0;
        while( offset < size )
        {
#ifdef WIN32

            auto ret = ::send(
                m_handle, reinterpret_cast< const uint8_t* >( data ) + offset, size - offset, 0 );

            if( ret == SOCKET_ERROR )
            {
                throw Error( Error::Type::BROKEN );
            }

#else

            auto ret = ::send( m_handle,
                               reinterpret_cast< const uint8_t* >( data ) + offset,
                               size - offset,
                               MSG_NOSIGNAL );

            if( ret == -1 )
            {
                if( errno != EAGAIN && errno != EWOULDBLOCK )
                {
                    throw Error( Error::Type::BROKEN, std::string( strerror( errno ) ) );
                }
            }

#endif

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
#ifdef WIN32

                ret = recv(
                    m_handle, reinterpret_cast< uint8_t* >( data ) + offset, size - offset, 0 );

                if( ret == SOCKET_ERROR )
                {
                    int errorNum = WSAGetLastError();
                    switch( errorNum )
                    {
                        case WSAEWOULDBLOCK:
                            break;
                        case WSAECONNRESET:
                            throw Error::CLOSED;
                        default:
                            throw Error::BROKEN;
                    }
                }
                else if( ret == 0 )
                    throw Error::CLOSED;
                else
                    break;

#else

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
                    switch( errno )
                    {
                        case EAGAIN:
                            break;
                        default:
                            throw Error( Error::Type::BROKEN );
                    }
                }

#endif
            }

            offset += ret;
        }
    }
}
