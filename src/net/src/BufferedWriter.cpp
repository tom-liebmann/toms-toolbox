#include <ttb/net/BufferedWriter.hpp>


namespace ttb
{
    BufferedWriter::BufferedWriter( size_t capacity )
    {
        m_data.reserve( capacity );
    }

    size_t BufferedWriter::capacity() const
    {
        return m_data.capacity();
    }

    size_t BufferedWriter::size() const
    {
        return m_data.size();
    }

    uint8_t const* BufferedWriter::data() const
    {
        return m_data.data();
    }

    void BufferedWriter::clear()
    {
        m_data.clear();
    }

    std::vector< uint8_t > BufferedWriter::release()
    {
        return std::move( m_data );
    }

    void BufferedWriter::write( void const* data, size_t size )
    {
        auto ptr = reinterpret_cast< uint8_t const* >( data );
        m_data.insert( std::end( m_data ), ptr, ptr + size );
    }
}
