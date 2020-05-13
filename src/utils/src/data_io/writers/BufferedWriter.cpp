#include <ttb/utils/data_io/writers/BufferedWriter.hpp>

#include <ttb/utils/data_io/packets/BufferPacket.hpp>


namespace ttb
{
    BufferedWriter::BufferedWriter( size_t capacity ) : m_data( capacity )
    {
    }

    size_t BufferedWriter::capacity() const
    {
        return m_data.capacity();
    }

    void BufferedWriter::clear()
    {
        m_data.clear();
    }

    BufferPacket BufferedWriter::packet() const
    {
        return { m_data.data(), m_data.size() };
    }

    size_t BufferedWriter::write( void const* data, size_t size )
    {
        auto const dataPtr = reinterpret_cast< uint8_t const* >( data );
        m_data.insert( std::end( m_data ), dataPtr, dataPtr + size );
        return size;
    }
}
