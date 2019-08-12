#pragma once

#include <ttb/net/BufferedWriter.hpp>
#include <ttb/net/packets.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/signal.hpp>


namespace ttb
{
    template < typename TEventDefinition >
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


namespace ttb
{
    template < typename TEventDefinition >
    PacketBridge< TEventDefinition >::PacketBridge()
        : m_packetInput( [this]( auto packet ) { onPacketInput( std::move( packet ) ); } )
        , m_eventInput( [this]( auto& event ) { onEventInput( event ); } )
        , m_readState( ReadState::SIZE )
        , m_readOffset( 0 )
    {
    }

    template < typename TEventDefinition >
    typename PacketBridge< TEventDefinition >::PacketInput const&
        PacketBridge< TEventDefinition >::packetInput()
    {
        return m_packetInput;
    }

    template < typename TEventDefinition >
    typename PacketBridge< TEventDefinition >::EventOutput&
        PacketBridge< TEventDefinition >::eventOutput()
    {
        return m_eventOutput;
    }

    template < typename TEventDefinition >
    typename PacketBridge< TEventDefinition >::EventInput const&
        PacketBridge< TEventDefinition >::eventInput() const
    {
        return m_eventInput;
    }

    template < typename TEventDefinition >
    typename PacketBridge< TEventDefinition >::DataOutput&
        PacketBridge< TEventDefinition >::dataOutput()
    {
        return m_dataOutput;
    }

    template < typename TEventDefinition >
    void PacketBridge< TEventDefinition >::reset()
    {
        auto lock = std::scoped_lock{ m_mutex };

        m_readState = ReadState::SIZE;
        m_readOffset = 0;
    }

    template < typename TEventDefinition >
    void PacketBridge< TEventDefinition >::onPacketInput(
        std::shared_ptr< ttb::OPacket const > packet )
    {
        auto dataWriter = BufferedWriter{ sizeof( uint32_t ) + packet->size() };

        // Write packet size
        auto packetSize = static_cast< uint32_t >( packet->size() );
        dataWriter.write( &packetSize, sizeof( uint32_t ) );

        // Write packet payload
        packet->write( dataWriter );

        m_dataOutput.call( dataWriter.release() );
    }

    template < typename TEventDefinition >
    void PacketBridge< TEventDefinition >::onEventInput( ttb::Event& event )
    {
        auto lock = std::unique_lock{ m_mutex };

        if( event.type() == TEventDefinition::DATA )
        {
            auto& e = static_cast< typename TEventDefinition::Data& >( event );

            switch( m_readState )
            {
                case ReadState::SIZE:
                {
                    m_readOffset += e.read( reinterpret_cast< uint8_t* >( &m_size ) + m_readOffset,
                                            sizeof( uint32_t ) - m_readOffset );

                    if( m_readOffset == sizeof( uint32_t ) )
                    {
                        m_data.resize( m_size );
                        m_readOffset = 0;
                        m_readState = ReadState::DATA;
                    }

                    break;
                }

                case ReadState::DATA:
                {
                    m_readOffset +=
                        e.read( m_data.data() + m_readOffset, m_data.size() - m_readOffset );

                    if( m_readOffset == m_data.size() )
                    {
                        m_readOffset = 0;
                        m_readState = ReadState::SIZE;

                        lock.unlock();

                        auto event = typename TEventDefinition::Packet{
                            std::make_unique< ttb::SizedIPacket >( std::move( m_data ) )
                        };

                        m_eventOutput.call( event );
                    }

                    break;
                }
            }
        }
        else
        {
            lock.unlock();

            m_eventOutput.call( event );
        }
    }
}
