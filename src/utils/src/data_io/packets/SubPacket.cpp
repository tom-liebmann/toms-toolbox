#include <ttb/utils/data_io/packets/SubPacket.hpp>

#include <cassert>


namespace ttb
{
    SubPacket::SubPacket( size_t offset, size_t size, Packet const& packet )
        : m_offset{ offset }, m_size{ size }, m_packet{ packet }
    {
        assert( m_offset + m_size <= m_packet.size() );
    }

    size_t SubPacket::size() const
    {
        return m_size;
    }

    size_t SubPacket::write( size_t offset, size_t size, Writer& writer ) const
    {
        assert( offset + size <= m_size );

        return m_packet.write( offset + m_offset, size, writer );
    }
}
