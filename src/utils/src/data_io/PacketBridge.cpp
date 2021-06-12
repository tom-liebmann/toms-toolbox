#include <ttb/utils/data_io/PacketBridge.hpp>

#include <ttb/utils/RingBufferWriter.hpp>
#include <ttb/utils/data_io/packets/BufferPacket.hpp>
#include <ttb/utils/data_io/writers/BufferWriter.hpp>


namespace ttb::net
{
    PacketBridge::PacketBridge() : m_buffer( sizeof( uint32_t ) )
    {
    }

    void PacketBridge::reset()
    {
        auto const lock = std::scoped_lock{ m_mutex };

        m_bytesRequired = sizeof( uint32_t );
        m_buffer.resize( sizeof( uint32_t ) );
        m_state = State::READ_SIZE;

        // Increase the run number so that pushData can detect a reset
        ++m_run;
    }

    void PacketBridge::pushData( ttb::Packet const& packet, PacketCallback const& callback )
    {
        auto lock = std::unique_lock{ m_mutex };

        auto const packetSize = packet.size();

        size_t offset = 0;

        while( offset < packetSize )
        {
            auto const bufferOffset = m_buffer.size() - m_bytesRequired;
            auto const blockSize = std::min( m_bytesRequired, packet.size() );

            auto writer = ttb::BufferWriter{ m_buffer.data() + bufferOffset, blockSize };
            auto const written = packet.write( offset, packetSize - offset, writer );

            offset += written;
            m_bytesRequired -= written;

            if( 0 == m_bytesRequired )
            {
                switch( m_state )
                {
                    case State::READ_SIZE:
                    {
                        m_bytesRequired = *reinterpret_cast< uint32_t const* >( m_buffer.data() );
                        m_state = State::READ_PAYLOAD;
                        break;
                    }

                    case State::READ_PAYLOAD:
                    {
                        m_bytesRequired = sizeof( uint32_t );
                        m_state = State::READ_SIZE;

                        if( callback )
                        {
                            auto const packet =
                                ttb::BufferPacket{ m_buffer.data(), m_buffer.size() };

                            auto const currentRun = m_run;
                            lock.unlock();

                            callback( packet );

                            lock.lock();
                            if( m_run != currentRun )
                            {
                                return;
                            }
                        }

                        break;
                    }
                }

                m_buffer.resize( m_bytesRequired );
            }
        }
    }
}
