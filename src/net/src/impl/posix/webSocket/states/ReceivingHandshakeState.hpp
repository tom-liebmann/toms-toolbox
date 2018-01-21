#pragma once

#include "SocketState.hpp"

#include <vector>


namespace ttb
{
    namespace posix
    {
        namespace webSocket
        {
            class ReceivingHandshakeState : public ttb::posix::webSocket::SocketState
            {
            public:
                ReceivingHandshakeState( ttb::posix::WebSocket& socket );

                // Override: State
                virtual bool isReadable() const override;
                virtual void doRead( std::shared_ptr< SelectableHolder > const& source,
                                     PushOutput< Event& >& eventOutput ) override;
                virtual bool isWritable() const override;
                virtual void doWrite( std::shared_ptr< SelectableHolder > const& source,
                                      PushOutput< Event& >& eventOutput ) override;

            private:
                std::vector< uint8_t > m_data;
            };
        }
    }
}
