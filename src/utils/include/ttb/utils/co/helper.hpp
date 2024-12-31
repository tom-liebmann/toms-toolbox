#pragma once

#include <functional>


namespace ttb::co
{
    template < typename TResult >
    class Promise;
}


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

    template < typename TType >
    struct IsCoroutinePromise
    {
        static constexpr bool value = false;
    };

    template < typename TResult >
    struct IsCoroutinePromise< Promise< TResult > >
    {
        static constexpr bool value = true;
    };
}
