#include <ttb/net/PacketBridge.hpp>

#include <ttb/net/events.hpp>

#include <iostream>


namespace ttb
{
    PacketBridge::PacketBridge()
        : m_packetInput( std::make_shared< PacketInput >(
              [this]( auto packet ) { this->onPacketInput( std::move( packet ) ); } ) )
        , m_eventInput( std::make_shared< EventInput >(
              [this]( auto& event ) { this->onEventInput( event ); } ) )
        , m_readState( ReadState::SIZE )
        , m_readOffset( 0 )
    {
    }

    std::shared_ptr< PacketBridge::PacketInput > const& PacketBridge::packetInput()
    {
        return m_packetInput;
    }

    PacketBridge::EventOutput& PacketBridge::eventOutput()
    {
        return m_eventOutput;
    }

    std::shared_ptr< PacketBridge::EventInput > const& PacketBridge::eventInput()
    {
        return m_eventInput;
    }

    PacketBridge::DataOutput& PacketBridge::dataOutput()
    {
        return m_dataOutput;
    }

    void PacketBridge::onEventInput( ttb::Event& event )
    {
        if( event.type() == ttb::events::DATA )
        {
            auto& e = static_cast< ttb::events::Data& >( event );

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

                        ttb::events::Packet event(
                            std::make_unique< ttb::SizedIPacket >( std::move( m_data ) ) );

                        m_eventOutput.push( event );
                    }

                    break;
                }
            }
        }
        else
        {
            m_eventOutput.push( event );
        }
    }

    void PacketBridge::onPacketInput( std::shared_ptr< ttb::OPacket const > packet )
    {
        uint32_t packetSize = packet->size();
        m_dataOutput.push( std::vector< uint8_t >( reinterpret_cast< uint8_t* >( &packetSize ),
                                                   reinterpret_cast< uint8_t* >( &packetSize ) +
                                                       sizeof( uint32_t ) ) );

        packet->send( m_dataOutput );
    }
}
