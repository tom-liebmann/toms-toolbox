#include <ttb/utils/data_io/RandomAccessPacket.hpp>

#include <ttb/utils/data_io/writers/BufferWriter.hpp>


namespace ttb
{
    size_t RandomAccessPacket::write( Writer& writer ) const
    {
        return writer.write( data(), size() );
    }
}
