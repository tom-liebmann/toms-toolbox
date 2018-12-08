#pragma once

#include "priv/call_converted_helper.hpp"


namespace ttb
{
    template < typename... TArgs2 >
    struct call_converted
    {
        template < typename TSignature, typename... TArgs1 >
        static auto call( TSignature const& function, std::tuple< TArgs1... >& args1 )
        {
            using ResultType = std::result_of_t< TSignature( TArgs2... ) >;

            if constexpr( std::is_same_v< void, ResultType > )
            {
                priv::call_converted_helper< ResultType,
                                             std::tuple< TArgs1... >,
                                             std::tuple< TArgs2... >,
                                             sizeof...( TArgs1 ) >::call( function, args1 );
            }
            else
            {
                return priv::call_converted_helper< ResultType,
                                                    std::tuple< TArgs1... >,
                                                    std::tuple< TArgs2... >,
                                                    sizeof...( TArgs1 ) >::call( function, args1 );
            }
        }
    };
}
