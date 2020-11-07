#include <ttb/utils/RingBufferWriter.hpp>


namespace ttb
{
    RingBufferWriter::RingBufferWriter( RingBuffer& buffer ) : m_buffer{ buffer }
    {
    }

    size_t RingBufferWriter::write( void const* data, size_t size )
    {
        return m_buffer.get().push( data, size );
    }
}
