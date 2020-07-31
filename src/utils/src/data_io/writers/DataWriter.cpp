#include <ttb/utils/data_io/writers/DataWriter.hpp>


namespace ttb
{
    DataWriter::DataWriter( Writer& writer, Endianness endianness )
        : m_endianness{ endianness }, m_writer{ writer }
    {
    }

    size_t DataWriter::write( void const* data, size_t size )
    {
        return m_writer.get().write( data, size );
    }
}
