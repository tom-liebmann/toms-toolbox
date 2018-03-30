#pragma once

#include "SocketState.hpp"

#include <vector>


namespace ttb
{
    namespace webSocket
    {
        class ServerHandshakeState : public ttb::webSocket::SocketState
        {
        public:
            ServerHandshakeState( ttb::WebSocket& socket );

            // Override: SocketState
            virtual void onEventInput( ttb::Event& event ) override;
            virtual void onDataInput( std::vector< uint8_t > data ) override;

        private:
            // During handshake, we do not send any data but buffer it for later use
            std::vector< std::vector< uint8_t > > m_sendBuffer;

            std::vector< uint8_t > m_data;
        };
    }
}
