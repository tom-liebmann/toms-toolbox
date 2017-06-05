#pragma once

#include <ttb/math/Tensor.hpp>

#include <cmath>
#include <utility>

// declarations
//=============================================================================

namespace ttb
{
    template < typename T, size_t D >
    using Vector = Tensor< T, D >;

    template < typename T, size_t D >
    T norm( Vector< T, D > const& vec );

    template < typename T, size_t D >
    T norm2( Vector< T, D > const& vec );

    template < typename T, size_t D >
    Vector< T, D > normalize( Vector< T, D > const& vec );

    template < typename T1, typename T2 >
    Vector< decltype( std::declval< T1 >() * std::declval< T2 >() ), 3 > cross(
        Vector< T1, 3 > const& lhs, Vector< T2, 3 > const& rhs );

    template < typename T1, typename T2, size_t D >
    decltype( std::declval< T1 >() * std::declval< T2 >() ) dot( Vector< T1, D > const& lhs,
                                                                 Vector< T2, D > const& rhs );
}



// definitions
//=============================================================================

namespace ttb
{
    template < typename T, size_t D >
    T norm( Vector< T, D > const& vec )
    {
        using std::sqrt;
        return sqrt( norm2( vec ) );
    }

    template < typename T, size_t D >
    T norm2( Vector< T, D > const& vec )
    {
        T result = 0;

        for( size_t i = 0; i < D; ++i )
            result += vec[ i ] * vec[ i ];

        return result;
    }

    template < typename T, size_t D >
    Vector< T, D > normalize( Vector< T, D > const& vec )
    {
        return vec / norm( vec );
    }

    template < typename T1, typename T2 >
    Vector< decltype( std::declval< T1 >() * std::declval< T2 >() ), 3 > cross(
        Vector< T1, 3 > const& lhs, Vector< T2, 3 > const& rhs )
    {
        return Vector< decltype( std::declval< T1 >() * std::declval< T2 >() ), 3 >(
            { lhs[ 1 ] * rhs[ 2 ] - lhs[ 2 ] * rhs[ 1 ], lhs[ 2 ] * rhs[ 0 ] - lhs[ 0 ] * rhs[ 2 ],
              lhs[ 0 ] * rhs[ 1 ] - lhs[ 1 ] * rhs[ 0 ] } );
    }

    template < typename T1, typename T2, size_t D >
    decltype( std::declval< T1 >() * std::declval< T2 >() ) dot( Vector< T1, D > const& lhs,
                                                                 Vector< T2, D > const& rhs )
    {
        decltype( std::declval< T1 >() * std::declval< T2 >() ) result = 0;

        for( size_t i = 0; i < D; ++i )
            result += lhs[ i ] * rhs[ i ];

        return result;
    }
}
