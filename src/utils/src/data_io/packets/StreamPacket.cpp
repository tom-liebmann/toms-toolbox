#include <ttb/utils/data_io/packets/StreamPacket.hpp>


namespace ttb
{
    StreamPacket::StreamPacket( RandomAccessPacket const& packet ) : m_packet{ packet }
    {
    }

    uint8_t const* StreamPacket::pop( size_t dstSize )
    {
        uint8_t const* result = nullptr;

        if( size() >= dstSize )
        {
            result = data();
            m_readHead += dstSize;
        }

        return result;
    }

    size_t StreamPacket::size() const
    {
        return m_packet.get().size() - m_readHead;
    }

    uint8_t const* StreamPacket::data() const
    {
        return m_packet.get().data() + m_readHead;
    }
}
