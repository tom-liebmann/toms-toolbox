#pragma once

#include <ttb/net/packets.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/dataIO.hpp>


namespace ttb
{
    class PacketBridge
    {
    public:
        using PacketInput = ttb::PushInput< std::shared_ptr< ttb::OPacket const > >;
        using PacketOutput = ttb::PushOutput< std::unique_ptr< ttb::IPacket > >;

        using EventInput = ttb::PushInput< ttb::Event& >;
        using DataOutput = ttb::PushOutput< std::vector< uint8_t > >;

        PacketBridge();

        std::shared_ptr< PacketInput > const& packetInput();
        PacketOutput& packetOutput();

        std::shared_ptr< EventInput > const& eventInput();
        DataOutput& dataOutput();

    private:
        void onEventInput( ttb::Event& event );
        void onPacketInput( std::shared_ptr< ttb::OPacket const > packet );

        std::shared_ptr< PacketInput > m_packetInput;
        PacketOutput m_packetOutput;

        std::shared_ptr< EventInput > m_eventInput;
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
    };
}
