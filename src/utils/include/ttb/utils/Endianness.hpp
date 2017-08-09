#pragma once

#include <algorithm>

namespace ttb
{
    enum class Endianness
    {
        BIG,
        LITTLE
    };

    Endianness nativeEndianness();

    template < typename TType >
    TType reverse( TType const& value );
}


namespace ttb
{
    template < typename TType >
    TType reverse( TType const& value )
    {
        TType rValue = value;
        std::reverse( reinterpret_cast< char* >( &rValue ),
                      reinterpret_cast< char* >( &rValue ) + sizeof( TType ) );
        return rValue;
    }
}
