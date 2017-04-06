#include "Listener.hpp"

#include <ttb/net/TCPSocket.hpp>

#ifdef WIN32
#else

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#endif

namespace ttb
{
    Listener::Listener( uint16_t port )
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

        // Initialize address structure.
        sockaddr_in sockAddr;

        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons( port );
        sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;

        // Bind port to socket.
        if( bind( m_handle, reinterpret_cast< sockaddr* >( &sockAddr ), sizeof( sockAddr ) ) )
        {
            auto error = WSAGetLastError();
            closesocket( m_handle );
            throw std::runtime_error( "Unable to bind port. (" + std::to_string( error ) + ")" );
        }

        // Set socket to listening mode.
        if( listen( m_handle, 10 ) )
        {
            auto error = WSAGetLastError();
            closesocket( m_handle );
            throw std::runtime_error( "Unable to listen on port. (" + std::to_string( error ) +
                                      ")" );
        }

#else

        // Create socket.
        m_handle = ::socket( AF_INET, SOCK_STREAM, 0 );

        if( m_handle == -1 )
            throw std::runtime_error( "Unable to create socket." );

        // Initialize address structure.
        sockaddr_in sockAddr;

        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons( port );
        sockAddr.sin_addr.s_addr = INADDR_ANY;

        // Bind port to socket.
        if( bind( m_handle, reinterpret_cast< sockaddr* >( &sockAddr ), sizeof( sockAddr ) ) )
        {
            close( m_handle );
            throw std::runtime_error( "Unable to bind port." );
        }

        // Set socket to listening mode.
        if( listen( m_handle, 10 ) )
        {
            close( m_handle );
            throw std::runtime_error( "Unable to listen on port." );
        }

#endif
    }

    Listener::~Listener()
    {
#ifdef WIN32

        closesocket( m_handle );

#else

        close( m_handle );

#endif
    }

    std::unique_ptr< TCPSocket > Listener::accept()
    {
#ifdef WIN32

        auto socket = ::accept( m_handle, NULL, NULL );

        if( socket == INVALID_SOCKET )
        {
            auto error = WSAGetLastError();
            throw std::runtime_error( "Error while waiting for connections. (" +
                                      std::to_string( error ) + ")" );
        }

        return std::unique_ptr< TCPSocket >( new TCPSocket( socket ) );

#else

        auto socket = ::accept( m_handle, NULL, NULL );

        if( socket == -1 )
            throw std::runtime_error( "Error while waiting for connections." );

        return std::unique_ptr< TCPSocket >( new TCPSocket( socket ) );

#endif
    }
}
