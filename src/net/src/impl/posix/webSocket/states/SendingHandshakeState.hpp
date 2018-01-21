#pragma once

#include "SocketState.hpp"

#include <string>


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
                virtual void doRead( std::shared_ptr< SelectableHolder > const& source,
                                     PushOutput< Event& >& eventOutput ) override;
                virtual bool isWritable() const override;
                virtual void doWrite( std::shared_ptr< SelectableHolder > const& source,
                                      PushOutput< Event& >& eventOutput ) override;

            private:
                size_t m_offset;
                std::string m_message;
            };
        }
    }
}
