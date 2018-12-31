#include <ttb/net/packets/SubIPacket.hpp>


namespace ttb
{
    SubIPacket::SubIPacket( size_t offset, IPacket const& packet )
        : m_offset( offset ), m_packet( packet )
    {
    }

    void const* SubIPacket::data() const
    {
        return reinterpret_cast< uint8_t const* >( m_packet.data() ) + m_offset;
    }

    size_t SubIPacket::size() const
    {
        return m_packet.size() - m_offset;
    }
}
