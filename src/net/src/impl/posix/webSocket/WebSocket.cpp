#include "WebSocket.hpp"
#include "states/ReceivingHandshakeState.hpp"
#include <ttb/net/netEvents.hpp>
#include <ttb/net/packets/SizedIPacket.hpp>
#include <ttb/utils/SHA1.hpp>
#include <ttb/utils/base64.hpp>

#include <iostream>
#include <regex>
#include <sstream>
#include <sys/socket.h>


namespace ttb
{
    std::shared_ptr< ttb::WebSocket > WebSocket::create( std::shared_ptr< ttb::TCPSocket > socket )
    {
        if( socket )
        {
            if( auto sck = std::dynamic_pointer_cast< ttb::posix::TCPSocket >( socket ) )
            {
                return std::make_shared< ttb::posix::WebSocket >( sck );
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


    namespace posix
    {
        WebSocket::WebSocket( std::shared_ptr< ttb::posix::TCPSocket > socket )
            : m_socket( std::move( socket ) )
        {
            state( std::make_unique< webSocket::ReceivingHandshakeState >( *this ) );
        }

        void WebSocket::state( std::unique_ptr< webSocket::SocketState > state )
        {
            m_state = std::move( state );
        }

        int WebSocket::handle() const
        {
            return m_socket->handle();
        }

        bool WebSocket::isReadable() const
        {
            return m_state->isReadable();
        }

        void WebSocket::doRead( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput )
        {
            m_state->doRead( eventOutput );
        }

        bool WebSocket::isWritable() const
        {
            return m_state->isWritable();
        }

        void WebSocket::doWrite( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput )
        {
            m_state->doWrite( eventOutput );
        }

        void WebSocket::send( std::shared_ptr< ttb::OPacket const > packet )
        {
        }
    }
}
