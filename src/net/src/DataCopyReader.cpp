#include <ttb/net/DataCopyReader.hpp>


namespace ttb
{
    DataCopyReader::DataCopyReader( DataReader& reader )
        : m_buffer( reader.avaliable() ), m_offset( 0 )
    {
        reader.read( m_buffer.data(), m_buffer.size() );
    }

    size_t DataCopyReader::available() const
    {
        return m_buffer.size() - m_offset;
    }

    size_t DataCopyReader::read( void* data, size_t size )
    {
        auto result = std::min( size, m_buffer.size() - m_offset );

        std::copy( std::next( m_buffer, m_offset ),
                   std::end( m_buffer ),
                   reinterpret_cast< uint8_t* >( data ) );

        return result;
    }
}
