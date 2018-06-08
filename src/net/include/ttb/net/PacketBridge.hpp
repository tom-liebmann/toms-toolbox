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
        using EventOutput = ttb::PushOutput< ttb::Event& >;

        using EventInput = ttb::PushInput< ttb::Event& >;
        using DataOutput = ttb::PushOutput< std::vector< uint8_t > >;

        template < typename TChild >
        PacketBridge( TChild& child );

        std::shared_ptr< PacketInput > const& packetInput();
        EventOutput& eventOutput();

        void reset();

    private:
        void onEventInput( ttb::Event& event );
        void onPacketInput( std::shared_ptr< ttb::OPacket const > packet );

        std::shared_ptr< PacketInput > m_packetInput;
        EventOutput m_eventOutput;

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


namespace ttb
{
    template < typename TChild >
    PacketBridge::PacketBridge( TChild& child )
        : m_packetInput( std::make_shared< PacketInput >(
              [this]( auto packet ) { this->onPacketInput( std::move( packet ) ); } ) )
        , m_eventInput( std::make_shared< EventInput >(
              [this]( auto& event ) { this->onEventInput( event ); } ) )
        , m_readState( ReadState::SIZE )
        , m_readOffset( 0 )
    {
        child.eventOutput().input( m_eventInput );
        m_dataOutput.input( child.dataInput() );
    }
}
