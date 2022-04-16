#pragma once

#include <cstdint>


namespace ttb
{
    template < typename TType >
    struct Hint
    {
    };

    constexpr Hint< float > use_float;
    constexpr Hint< std::uint8_t > use_uint8;
}
