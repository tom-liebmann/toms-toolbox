#pragma once

#include <ttb/utils/RingBuffer.hpp>
#include <ttb/utils/data_io/Packet.hpp>
#include <ttb/utils/data_io/RandomAccessPacket.hpp>
#include <ttb/utils/signal.hpp>

#include <mutex>


namespace ttb
{
    class PacketBridge
    {
    public:
        using PacketCallback = std::function< void( ttb::RandomAccessPacket const& ) >;

        PacketBridge();

        void reset();

        void pushData( ttb::Packet const& packet, PacketCallback const& callback );

    private:
        enum class State
        {
            READ_SIZE,
            READ_PAYLOAD,
        };

        PacketCallback m_packetCallback;

        State m_state{ State::READ_SIZE };

        size_t m_bytesRequired{ sizeof( uint32_t ) };

        std::vector< uint8_t > m_buffer;

        uint32_t m_run{ 0 };

        std::mutex m_mutex;
    };
}
