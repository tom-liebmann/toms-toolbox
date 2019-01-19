#pragma once

#include <ttb/net/packets.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/signal.hpp>


namespace ttb
{
    class PacketBridge
    {
    public:
        using PacketInput = ttb::Slot< void( std::shared_ptr< ttb::OPacket const > ) >;
        using EventOutput = ttb::Signal< void( ttb::Event& ) >;
        using EventInput = ttb::Slot< void( ttb::Event& ) >;
        using DataOutput = ttb::Signal< void( std::vector< uint8_t > ) >;

        PacketBridge();

        PacketInput const& packetInput();
        EventOutput& eventOutput();

        EventInput const& eventInput() const;
        DataOutput& dataOutput();

        void reset();

    private:
        void onPacketInput( std::shared_ptr< ttb::OPacket const > packet );
        void onEventInput( ttb::Event& event );

        // External connections
        PacketInput m_packetInput;
        EventOutput m_eventOutput;
        EventInput m_eventInput;
        DataOutput m_dataOutput;

        enum class ReadState
        {
            SIZE,
            DATA
        };

        ReadState m_readState;
        size_t m_readOffset;
        uint32_t m_size;
        std::vector< uint8_t > m_data;

        std::mutex m_mutex;
    };
}
