#pragma once

#include "convert_to.hpp"

#include <memory>


namespace ttb
{
    template < typename TSource, typename TTarget >
    struct convert_to< std::shared_ptr< TSource >,
                       TTarget,
                       std::enable_if_t< std::is_convertible_v< TSource&, TTarget > > >
    {
        static TTarget call( std::shared_ptr< TSource > const& source )
        {
            return convert_to< TSource&, TTarget >::call( *source );
        }
    };

    template < typename TSource, typename TTarget >
    struct convert_to< std::unique_ptr< TSource >, TTarget >
    {
        static TTarget call( std::unique_ptr< TSource > source );
    };

    template < typename TSource, typename TTarget >
    struct convert_to< std::unique_ptr< TSource >, std::shared_ptr< TTarget > >
    {
        static std::shared_ptr< TTarget > call( std::unique_ptr< TSource > source );
    };
}


namespace ttb
{
    template < typename TSource, typename TTarget >
    inline TTarget
        convert_to< std::unique_ptr< TSource >, TTarget >::call( std::unique_ptr< TSource > source )
    {
        return convert_to< TSource&, TTarget >::call( *source );
    }

    template < typename TSource, typename TTarget >
    inline std::shared_ptr< TTarget >
        convert_to< std::unique_ptr< TSource >, std::shared_ptr< TTarget > >::call(
            std::unique_ptr< TSource > source )
    {
        return std::shared_ptr< TTarget >( std::move( source ) );
    }
}
