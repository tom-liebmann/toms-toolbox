#pragma once

#include <algorithm>
#include <cstdint>

// declarations
//=================================================================================================

namespace tg
{
    enum class Endianess
    {
        BIGENDIAN,
        LITTLEENDIAN
    };

    Endianess nativeEndianess();

    template < typename T >
    T reverse( T const& value );
}



// definitions
//=================================================================================================

namespace tg
{
    template < typename T >
    T reverse( T const& value )
    {
        T result = value;

        std::reverse( reinterpret_cast< uint8_t* >( &result ),
                      reinterpret_cast< uint8_t* >( &result ) + sizeof( T ) );

        return result;
    }
}
