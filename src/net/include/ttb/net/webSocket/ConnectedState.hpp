#pragma once

#include "SocketState.hpp"

#include <vector>


namespace ttb
{
    namespace webSocket
    {
        class ConnectedState : public ttb::webSocket::SocketState
        {
        public:
            ConnectedState( WebSocket& socket );

            // Override: SocketState
            virtual void onEventInput( ttb::Event& event );
            virtual void onDataInput( std::vector< uint8_t > data );

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

            size_t m_writeOffset;

            std::vector< uint8_t > m_packetData;
        };
    }
}
