#include "WebSocket.hpp"
#include <ttb/net/netEvents.hpp>

#include <iostream>
#include <sys/socket.h>


namespace ttb
{
    std::shared_ptr< ttb::WebSocket > WebSocket::create( std::shared_ptr< ttb::TCPSocket > socket )
    {
        if( socket )
        {
            if( auto sck = std::dynamic_pointer_cast< ttb::linux::TCPSocket >( socket ) )
            {
                return std::make_shared< ttb::linux::WebSocket >( sck );
            }
            else
            {
                throw std::runtime_error( "Invalid socket type" );
            }
        }
        else
        {
            throw std::runtime_error( "Null socket" );
        }
    }


    namespace linux
    {
        WebSocket::WebSocket( std::shared_ptr< ttb::linux::TCPSocket > socket )
            : m_socket( std::move( socket ) )
        {
        }

        int WebSocket::handle() const
        {
            return m_socket->handle();
        }

        bool WebSocket::isReadable() const
        {
            return true;
        }

        void WebSocket::doRead( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput )
        {
            uint8_t buffer[ 256 ];

            auto result = ::recv( m_socket->handle(), buffer, 255, MSG_DONTWAIT );

            if( result < 0 )
            {
                if( errno != EAGAIN && errno != EWOULDBLOCK )
                {
                    throw std::runtime_error( "Connection lost" );
                }
            }
            else if( result == 0 )
            {
                ttb::events::SocketClosedEvent event( shared_from_this() );
                eventOutput.push( event );
            }
            else
            {
                buffer[ result ] = '\0';
                std::cout << "Data: " << buffer << std::endl;
            }
        }

        bool WebSocket::isWritable() const
        {
            return false;
        }

        void WebSocket::doWrite( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput )
        {
        }

        void WebSocket::send( std::shared_ptr< ttb::OPacket const > packet )
        {
        }
    }
}
