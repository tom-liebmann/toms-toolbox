#include <ttb/utils/data_io/Packet.hpp>

#include <ttb/utils/data_io/writers/BufferWriter.hpp>


namespace ttb
{
    Packet::~Packet()
    {
    }

    size_t Packet::write( void* dst, size_t dstSize ) const
    {
        BufferWriter writer{ dst, dstSize };
        return write( writer );
    }
}
