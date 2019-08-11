#pragma once

#include <ttb/math/Tensor.hpp>

#include <cmath>
#include <utility>


namespace ttb
{
    namespace impl
    {
        template < typename TType >
        constexpr TType HOMOGENIZE_THRESHOLD = static_cast< TType >( 1e-8 );
    }
}


// declarations
//=============================================================================

namespace ttb
{
    template < typename TType, size_t TDimension >
    using Vector = Tensor< TType, TDimension >;

    template < typename TType, size_t TDimension >
    auto norm( Vector< TType, TDimension > const& vec );

    template < typename TType, size_t TDimension >
    auto norm2( Vector< TType, TDimension > const& vec );

    template < typename TType, size_t TDimension >
    auto normalize( Vector< TType, TDimension > const& vec );

    template < typename TType1, typename TType2 >
    auto cross( Vector< TType1, 3 > const& lhs, Vector< TType2, 3 > const& rhs );

    template < typename TType1, typename TType2, size_t TDimension >
    auto dot( Vector< TType1, TDimension > const& lhs, Vector< TType2, TDimension > const& rhs );

    template < typename TType >
    Vector< TType, 4 > homogenize( Vector< TType, 3 > const& vec );

    template < typename TType >
    Vector< TType, 3 > homogenize( Vector< TType, 4 > const& vec );
}


// definitions
//=============================================================================

namespace ttb
{
    template < typename TType, size_t TDimension >
    auto norm( Vector< TType, TDimension > const& vec )
    {
        using std::sqrt;
        return sqrt( norm2( vec ) );
    }

    template < typename TType, size_t TDimension >
    auto norm2( Vector< TType, TDimension > const& vec )
    {
        TType result = 0;

        for( size_t d = 0; d < TDimension; ++d )
        {
            result += vec[ d ] * vec[ d ];
        }

        return result;
    }

    template < typename TType, size_t TDimension >
    auto normalize( Vector< TType, TDimension > const& vec )
    {
        return vec / norm( vec );
    }

    template < typename TType1, typename TType2 >
    auto cross( Vector< TType1, 3 > const& lhs, Vector< TType2, 3 > const& rhs )
    {
        using ElementType = decltype( std::declval< TType1 >() * std::declval< TType2 >() );

        return Vector< ElementType, 3 >( { lhs[ 1 ] * rhs[ 2 ] - lhs[ 2 ] * rhs[ 1 ],
                                           lhs[ 2 ] * rhs[ 0 ] - lhs[ 0 ] * rhs[ 2 ],
                                           lhs[ 0 ] * rhs[ 1 ] - lhs[ 1 ] * rhs[ 0 ] } );
    }

    template < typename TType1, typename TType2, size_t TDimension >
    auto dot( Vector< TType1, TDimension > const& lhs, Vector< TType2, TDimension > const& rhs )
    {
        using ReturnType = decltype( std::declval< TType1 >() * std::declval< TType2 >() );

        ReturnType result = 0;

        for( size_t d = 0; d < TDimension; ++d )
        {
            result += lhs[ d ] * rhs[ d ];
        }

        return result;
    }

    template < typename TType >
    Vector< TType, 4 > homogenize( Vector< TType, 3 > const& vec )
    {
        return { vec( 0 ), vec( 1 ), vec( 2 ), static_cast< TType >( 1 ) };
    }

    template < typename TType >
    Vector< TType, 3 > homogenize( Vector< TType, 4 > const& vec )
    {
        using std::abs;

        return Vector< TType, 3 >{ vec( 0 ), vec( 1 ), vec( 2 ) } /
               ( abs( vec( 3 ) ) > impl::HOMOGENIZE_THRESHOLD< TType >
                     ? vec( 3 )
                     : static_cast< TType >( 1 ) );
    }
}
