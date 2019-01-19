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

        template < typename TChild >
        PacketBridge( TChild& child );

        PacketBridge();

        PacketInput const& packetInput();
        EventOutput& eventOutput();

        void reset();

    private:
        void onPacketInput( std::shared_ptr< ttb::OPacket const > packet );
        void onEventInput( ttb::Event& event );

        // External connections
        PacketInput m_packetInput;
        EventOutput m_eventOutput;

        // Internal connections
        using EventInput = ttb::Slot< void( ttb::Event& ) >;
        using DataOutput = ttb::Signal< void( std::vector< uint8_t > ) >;
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


namespace ttb
{
    template < typename TChild >
    PacketBridge::PacketBridge( TChild& child )
        : m_packetInput( [this]( auto packet ) { this->onPacketInput( std::move( packet ) ); } )
        , m_eventInput( [this]( auto& event ) { this->onEventInput( event ); } )
        , m_readState( ReadState::SIZE )
        , m_readOffset( 0 )
    {
        connect( child.eventOutput(), m_eventInput );
        connect( m_dataOutput, child.dataInput() );
    }
}
