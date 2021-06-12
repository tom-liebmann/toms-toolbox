#include <ttb/utils/data_io/packets/CopiedPacket.hpp>

#include <ttb/utils/data_io/writers/BufferWriter.hpp>


namespace ttb
{
    CopiedPacket::CopiedPacket( ttb::Packet const& original ) : m_data( original.size() )
    {
        auto writer = BufferWriter{ m_data.data(), m_data.size() };
        original.write( 0, m_data.size(), writer );
    }

    size_t CopiedPacket::size() const
    {
        return m_data.size();
    }

    uint8_t const* CopiedPacket::data() const
    {
        return m_data.data();
    }
}
