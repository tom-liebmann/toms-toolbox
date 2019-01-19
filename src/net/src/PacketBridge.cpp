#include <ttb/net/PacketBridge.hpp>

#include <ttb/net/DataWriter.hpp>
#include <ttb/net/events.hpp>

#include <iostream>


namespace
{
    class PacketDataWriter : public ttb::DataWriter
    {
    public:
        PacketDataWriter( size_t size );

        virtual void write( void const* data, size_t size ) override;

        std::vector< uint8_t >& data();

    private:
        std::vector< uint8_t > m_data;
    };
}


namespace ttb
{
    PacketBridge::PacketBridge()
        : m_packetInput( [this]( auto packet ) { this->onPacketInput( std::move( packet ) ); } )
        , m_eventInput( [this]( auto& event ) { this->onEventInput( event ); } )
        , m_readState( ReadState::SIZE )
        , m_readOffset( 0 )
    {
    }

    PacketBridge::PacketInput const& PacketBridge::packetInput()
    {
        return m_packetInput;
    }

    PacketBridge::EventOutput& PacketBridge::eventOutput()
    {
        return m_eventOutput;
    }

    void PacketBridge::reset()
    {
        std::lock_guard< std::mutex > lock( m_mutex );
        m_readState = ReadState::SIZE;
        m_readOffset = 0;
    }

    void PacketBridge::onPacketInput( std::shared_ptr< ttb::OPacket const > packet )
    {
        PacketDataWriter dataWriter( sizeof( uint32_t ) + packet->size() );

        // Write packet size
        auto packetSize = static_cast< uint32_t >( packet->size() );
        dataWriter.write( &packetSize, sizeof( uint32_t ) );

        // Write packet payload
        packet->write( dataWriter );

        m_dataOutput.call( std::move( dataWriter.data() ) );
    }

    void PacketBridge::onEventInput( ttb::Event& event )
    {
        std::unique_lock< std::mutex > lock( m_mutex );

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

                        lock.unlock();

                        ttb::events::Packet event(
                            std::make_unique< ttb::SizedIPacket >( std::move( m_data ) ) );

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


namespace
{
    PacketDataWriter::PacketDataWriter( size_t size )
    {
        m_data.reserve( size );
    }

    void PacketDataWriter::write( void const* data, size_t size )
    {
        m_data.insert( std::end( m_data ),
                       reinterpret_cast< uint8_t const* >( data ),
                       reinterpret_cast< uint8_t const* >( data ) + size );
    }

    std::vector< uint8_t >& PacketDataWriter::data()
    {
        return m_data;
    }
}
