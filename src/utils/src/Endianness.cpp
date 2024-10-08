#include <ttb/utils/Endianness.hpp>

#include <cstdint>


namespace
{
    ttb::Endianness computeNativeEndianness();
}


namespace ttb
{
    Endianness nativeEndianness()
    {
        static Endianness const endianness = computeNativeEndianness();
        return endianness;
    }
}


namespace
{
    ttb::Endianness computeNativeEndianness()
    {
        uint16_t value = 0x1234;
        // cppcheck-suppress knownConditionTrueFalse
        if( *reinterpret_cast< uint8_t* >( &value ) == 0x12 )
        {
            return ttb::Endianness::BIG;
        }
        else
        {
            return ttb::Endianness::LITTLE;
        }
    }
}
