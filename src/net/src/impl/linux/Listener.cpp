#include "Listener.hpp"
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


namespace ttb
{
    std::unique_ptr< Listener > Listener::create( uint16_t port )
    {
        return std::make_unique< linux::Listener >( port );
    }


    namespace linux
    {
        Listener::Listener( uint16_t port ) : ttb::Listener( port )
        {
            // Create socket.
            m_handle = ::socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );

            if( m_handle == -1 )
            {
                throw std::runtime_error( "Unable to create socket" );
            }

            // Initialize address structure.
            sockaddr_in sockAddr;

            sockAddr.sin_family = AF_INET;
            sockAddr.sin_port = htons( port );
            sockAddr.sin_addr.s_addr = INADDR_ANY;

            // Bind port to socket.
            if( bind( m_handle, reinterpret_cast< sockaddr* >( &sockAddr ), sizeof( sockAddr ) ) )
            {
                close( m_handle );
                throw std::runtime_error( "Unable to bind port " + std::to_string( port ) );
            }

            // Set socket to listening mode.
            if( listen( m_handle, 10 ) )
            {
                close( m_handle );
                throw std::runtime_error( "Unable to listen on port " + std::to_string( port ) );
            }
        }

        Listener::~Listener()
        {
            close( m_handle );
        }

        std::unique_ptr< ttb::TCPSocket > Listener::accept()
        {
            auto socketHandle = ::accept( m_handle, NULL, NULL );

            if( socketHandle == -1 )
            {
                throw std::runtime_error( "Error while waiting for connections" );
            }

            return std::make_unique< linux::TCPSocket >( socketHandle );
        }
    }
}
