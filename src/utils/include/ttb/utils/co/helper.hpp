#pragma once

#include <functional>


namespace ttb::co::helper
{
    template < typename TType >
    struct CallbackType
    {
        using type = std::function< void( TType const& ) >;
    };

    template <>
    struct CallbackType< void >
    {
        using type = std::function< void() >;
    };
}
