#pragma once


namespace ttb
{
    namespace priv
    {
        template < typename TResult, typename TArgs1, typename TArgs2, size_t TIndex >
        struct call_converted_helper
        {
            template < typename TSignature, typename... TArgs3 >
            static TResult call( TSignature const& function, TArgs1& args1, TArgs3&&... args3 );
        };

        template < typename TResult, typename TArgs1, typename TArgs2 >
        struct call_converted_helper< TResult, TArgs1, TArgs2, 0 >
        {
            template < typename TSignature, typename... TArgs3 >
            static TResult call( TSignature const& function, TArgs1& args1, TArgs3&&... args3 );
        };
    }
}


namespace ttb
{
    namespace priv
    {
        template < typename TResult, typename TArgs1, typename TArgs2, size_t TIndex >
        template < typename TSignature, typename... TArgs3 >
        inline TResult call_converted_helper< TResult, TArgs1, TArgs2, TIndex >::call(
            TSignature const& function, TArgs1& args1, TArgs3&&... args3 )
        {
            using Type1 = std::tuple_element_t< TIndex - 1, TArgs1 >;
            using Type2 = std::tuple_element_t< TIndex - 1, TArgs2 >;

            if constexpr( std::is_same_v< void, TResult > )
            {
                call_converted_helper< TResult, TArgs1, TArgs2, TIndex - 1 >::call(
                    function,
                    args1,
                    convert_to< Type1, Type2 >::call(
                        std::forward< Type1 >( std::get< TIndex - 1 >( args1 ) ) ),
                    std::forward< TArgs3 >( args3 )... );
            }
            else
            {
                return call_converted_helper< TResult, TArgs1, TArgs2, TIndex - 1 >::call(
                    function,
                    args1,
                    convert_to< Type1, Type2 >::call(
                        std::forward< Type1 >( std::get< TIndex - 1 >( args1 ) ) ),
                    std::forward< TArgs3 >( args3 )... );
            }
        }

        template < typename TResult, typename TArgs1, typename TArgs2 >
        template < typename TSignature, typename... TArgs3 >
        inline TResult call_converted_helper< TResult, TArgs1, TArgs2, 0 >::call(
            TSignature const& function, TArgs1& args1, TArgs3&&... args3 )
        {
            if constexpr( std::is_same_v< void, TResult > )
            {
                function( std::forward< TArgs3 >( args3 )... );
            }
            else
            {
                return function( std::forward< TArgs3 >( args3 )... );
            }
        }
    }
}
