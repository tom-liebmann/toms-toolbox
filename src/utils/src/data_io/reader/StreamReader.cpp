#include <ttb/utils/data_io/reader/StreamReader.hpp>


namespace ttb
{
    StreamReader::StreamReader( std::istream& stream ) : m_stream{ stream }
    {
    }

    bool StreamReader::read( void* buffer, std::size_t size )
    {
        return !m_stream.read( static_cast< char* >( buffer ), size ).fail();
    }
}
