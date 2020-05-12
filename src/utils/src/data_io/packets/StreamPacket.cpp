#include <ttb/utils/data_io/packets/StreamPacket.hpp>


namespace ttb
{
    StreamPacket::StreamPacket( RandomAccessPacket const& packet ) : m_packet{ packet }
    {
    }

    bool StreamPacket::pop( void* dst, size_t dstSize )
    {
        auto const written = m_packet.get().write( m_readHead, dstSize, dst, dstSize );

        if( written == dstSize )
        {
            m_readHead += dstSize;
            return true;
        }
        else
        {
            return false;
        }
    }

    size_t StreamPacket::size() const
    {
        return m_packet.get().size() - m_readHead;
    }

    size_t StreamPacket::write( size_t offset, size_t size, Writer& writer ) const
    {
        return m_packet.get().write( m_readHead + offset, size, writer );
    }
}
