#include "WebSocket.hpp"
#include "states/ReceivingHandshakeState.hpp"
#include <ttb/net/events.hpp>
#include <ttb/net/packets/SizedIPacket.hpp>
#include <ttb/net/packets/WebSocketOPacket.hpp>
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

        void WebSocket::doRead( std::shared_ptr< SelectableHolder > const& source,
                                PushOutput< Event& >& eventOutput )
        {
            m_state->doRead( source, eventOutput );
        }

        bool WebSocket::isWritable() const
        {
            return m_state->isWritable();
        }

        void WebSocket::doWrite( std::shared_ptr< SelectableHolder > const& source,
                                 PushOutput< Event& >& eventOutput )
        {
            m_state->doWrite( source, eventOutput );
        }

        void WebSocket::send( std::shared_ptr< ttb::OPacket const > packet )
        {
            std::lock_guard< std::mutex > lock( m_mutex );
            m_packets.push( std::make_shared< ttb::WebSocketOPacket >( std::move( packet ) ) );
        }
    }
}
