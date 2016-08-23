#pragma once

#include <cstdlib>

namespace tg_private
{
    template < size_t D0, size_t... D >
    class pack_product
    {
    public:
        static const size_t value = D0 * pack_product< D... >::value;
    };

    template < size_t D0 >
    class pack_product< D0 >
    {
    public:
        static const size_t value = D0;
    };



    template < size_t D0, size_t... D >
    class arg_product
    {
    public:
        template < typename T0, typename... T >
        static size_t value( T0 i0, T... i )
        {
            return i0 * pack_product< D... >::value + arg_product< D... >::value( i... );
        }
    };

    template < size_t D0 >
    class arg_product< D0 >
    {
    public:
        template < typename T0 >
        static size_t value( T0 i0 )
        {
            return i0;
        }
    };
}
