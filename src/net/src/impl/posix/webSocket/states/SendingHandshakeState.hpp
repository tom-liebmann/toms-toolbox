#pragma once

#include "SocketState.hpp"


namespace ttb
{
    namespace posix
    {
        namespace webSocket
        {
            class SendingHandshakeState : public ttb::posix::webSocket::SocketState
            {
            public:
                SendingHandshakeState( std::string handshakeMessage,
                                       ttb::posix::WebSocket& socket );

                // Override: State
                virtual bool isReadable() const override;
                virtual void doRead( ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >&
                                         eventOutput ) override;
                virtual bool isWritable() const override;
                virtual void doWrite( ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >&
                                          eventOutput ) override;

            private:
                size_t m_offset;
                std::string m_message;
            };
        }
    }
}
