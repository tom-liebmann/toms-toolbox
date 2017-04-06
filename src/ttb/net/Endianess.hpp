#pragma once

#include <algorithm>
#include <cstdint>

// declarations
//=================================================================================================

namespace ttb
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

namespace ttb
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
