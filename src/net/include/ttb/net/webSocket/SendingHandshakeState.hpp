#pragma once


namespace ttb
{
    namespace webSocket
    {
        class SendingHandshakeState : public ttb::webSocket::SocketState
        {
        public:
            SendingHandshakeState( WebSocket& socket, std::string handshakeMessage );

            // override: SocketState
            virtual void onEventInput( ttb::Event& event ) override;
            virtual void onDataInput( std::vector< uint8_t > data ) override;

        private:
            std::vector< std::vector< uint8_t > > m_sendBuffer;
        };
    }
}
