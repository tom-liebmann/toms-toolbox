#include <ttb/utils/data_io/writers/StreamWriter.hpp>


namespace ttb
{
    StreamWriter::StreamWriter( std::ostream& stream ) : m_stream{ stream }
    {
    }

    void StreamWriter::flush()
    {
        m_stream.get().flush();
    }

    size_t StreamWriter::write( void const* data, size_t size )
    {
        m_stream.get().write( reinterpret_cast< char const* >( data ), size );
        return size;
    }
}
