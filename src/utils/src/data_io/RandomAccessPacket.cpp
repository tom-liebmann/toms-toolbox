#include <ttb/utils/data_io/RandomAccessPacket.hpp>

#include <ttb/utils/data_io/writers/BufferWriter.hpp>


namespace ttb
{
    size_t RandomAccessPacket::write( size_t offset, Writer& writer ) const
    {
        auto const dataSize = size();

        if( offset > dataSize )
        {
            return 0;
        }
        else
        {
            return writer.write( data() + offset, size() - offset );
        }
    }
}
