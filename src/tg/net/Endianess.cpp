#include "Endianess.hpp"

#include <cstdint>

namespace
{
    tg::Endianess computeNativeEndianess()
    {
        uint16_t value = 0x1234;
        if( *reinterpret_cast< uint8_t* >( &value ) == 0x12 )
            return tg::Endianess::BIGENDIAN;
        else
            return tg::Endianess::LITTLEENDIAN;
    }
}

namespace tg
{
    Endianess nativeEndianess()
    {
        static Endianess endianess = computeNativeEndianess();
        return endianess;
    }
}
