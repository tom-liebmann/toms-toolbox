#include <ttb/utils/data_io/writers/BufferWriter.hpp>

#include <ttb/utils/data_io/packets/BufferPacket.hpp>

#include <algorithm>
#include <cmath>


namespace ttb
{
    BufferWriter::BufferWriter( void* buffer, size_t bufferSize )
        : m_buffer{ reinterpret_cast< uint8_t* >( buffer ) }, m_bufferSize{ bufferSize }
    {
    }

    size_t BufferWriter::capacity() const
    {
        return m_bufferSize;
    }

    void BufferWriter::clear()
    {
        m_writeHead = 0;
    }

    BufferPacket BufferWriter::packet() const
    {
        return { m_buffer, m_writeHead };
    }

    size_t BufferWriter::write( void const* data, size_t size )
    {
        auto const dataPtr = reinterpret_cast< uint8_t const* >( data );
        auto const copySize = std::min( size, m_bufferSize - m_writeHead );

        std::copy( dataPtr, dataPtr + copySize, m_buffer + m_writeHead );
        m_writeHead += copySize;

        return copySize;
    }
}
