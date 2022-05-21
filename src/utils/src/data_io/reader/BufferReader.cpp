#include <ttb/utils/data_io/reader/BufferReader.hpp>

#include <algorithm>


namespace ttb
{
    BufferReader::BufferReader( RandomAccessPacket const& packet ) : m_packet{ packet }
    {
    }

    bool BufferReader::read( void* buffer, std::size_t size )
    {
        if( m_packet.size() - m_readHead < size )
        {
            return false;
        }

        auto const srcPtr = m_packet.data() + m_readHead;
        std::copy( srcPtr, srcPtr + size, reinterpret_cast< std::uint8_t* >( buffer ) );

        m_readHead += size;

        return true;
    }
}
