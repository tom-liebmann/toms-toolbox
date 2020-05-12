#include <ttb/utils/data_io/packets/BufferPacket.hpp>

#include <ttb/utils/data_io/Writer.hpp>


namespace ttb
{
    BufferPacket::BufferPacket( void const* data, size_t dataSize )
        : m_data{ reinterpret_cast< uint8_t const* >( data ) }, m_dataSize{ dataSize }
    {
    }

    size_t BufferPacket::size() const
    {
        return m_dataSize;
    }

    size_t BufferPacket::write( size_t offset, size_t size, Writer& writer ) const
    {
        return writer.write( m_data + offset, size );
    }
}
