#include "Endianess.hpp"

#include <cstdint>

namespace
{
    ttb::Endianess computeNativeEndianess()
    {
        uint16_t value = 0x1234;
        if( *reinterpret_cast< uint8_t* >( &value ) == 0x12 )
            return ttb::Endianess::BIGENDIAN;
        else
            return ttb::Endianess::LITTLEENDIAN;
    }
}

namespace ttb
{
    Endianess nativeEndianess()
    {
        static Endianess endianess = computeNativeEndianess();
        return endianess;
    }
}
