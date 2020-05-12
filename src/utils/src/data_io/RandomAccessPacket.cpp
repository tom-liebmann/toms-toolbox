#include <ttb/utils/data_io/RandomAccessPacket.hpp>

#include <ttb/utils/data_io/writers/BufferWriter.hpp>


namespace ttb
{
    size_t RandomAccessPacket::write( size_t offset, size_t size, void* dst, size_t dstSize ) const
    {
        BufferWriter writer{ dst, dstSize };
        return write( offset, size, writer );
    }

    size_t RandomAccessPacket::write( Writer& writer ) const
    {
        return write( 0, size(), writer );
    }
}
