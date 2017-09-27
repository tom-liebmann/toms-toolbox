#include "Listener.hpp"
#include "TCPSocket.hpp"


namespace ttb
{
    std::unique_ptr< TCPSocket > Listener::create( uint16_t port )
    {
        return std::make_unique< linux::TCPSocket >( port );
    }

    Listener::Listener( uint16_t port ) : m_port( port )
    {
    }

    Listener::~Listener() = default;

    uint16_t Listener::port() const
    {
        return m_port;
    }
}


namespace ttb
{
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

        std::unique_ptr< TCPSocket > accept()
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
