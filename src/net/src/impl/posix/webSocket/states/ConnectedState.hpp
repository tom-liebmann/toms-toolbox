#pragma once

#include "SocketState.hpp"

#include <vector>


namespace ttb
{
    namespace posix
    {
        namespace webSocket
        {
            class ConnectedState : public ttb::posix::webSocket::SocketState
            {
            public:
                ConnectedState( ttb::posix::WebSocket& socket );

                // Override: SocketState
                virtual bool isReadable() const override;
                virtual void doRead( ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >&
                                         eventOutput ) override;
                virtual bool isWritable() const override;
                virtual void doWrite( ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >&
                                          eventOutput ) override;

            private:
                enum class ReadState
                {
                    READ_START,
                    READ_EXTENDED_LEN,
                    READ_MASKING_KEY,
                    READ_PAYLOAD
                };

                ReadState m_readState;

                bool m_lastFragment;

                bool m_isMasked;
                uint32_t m_maskingKey;

                uint64_t m_payloadLength;

                size_t m_readOffset;
                std::vector< uint8_t > m_readData;

                std::vector< uint8_t > m_packetData;
            };
        }
    }
}
