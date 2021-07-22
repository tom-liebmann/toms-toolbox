#include <ttb/utils/data_io/RandomAccessPacket.hpp>

#include <ttb/utils/data_io/writers/BufferWriter.hpp>

#include <cmath>
#include <algorithm>


namespace ttb
{
    size_t RandomAccessPacket::write( size_t offset, size_t size, Writer& writer ) const
    {
        auto const dataSize = this->size();

        if( offset > dataSize )
        {
            return 0;
        }
        else
        {
            return writer.write( data() + offset, std::min( size, dataSize - offset ) );
        }
    }
}
