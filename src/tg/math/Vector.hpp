#pragma once

#include <tg/math/Tensor.hpp>

#include <cmath>



// declarations
//=============================================================================

namespace tg
{
    template< typename T, size_t D >
    using Vector = Tensor< T, D >;

    template< typename T, size_t D >
    T norm( const Vector< T, D >& vec );

    template< typename T, size_t D >
    T norm2( const Vector< T, D >& vec );

    template< typename T1, typename T2, size_t D >
    Vector< decltype( T1() * T2() ), D > cross(
        const Vector< T1, D >& lhs,
        const Vector< T2, D >& rhs );

    template< typename T1, typename T2, size_t D >
    decltype( T1() * T2() ) dot(
        const Vector< T1, D >& lhs,
        const Vector< T2, D >& rhs );
}



// definitions
//=============================================================================

namespace tg
{
    template< typename T, size_t D >
    T norm( const Vector< T, D >& vec )
    {
        return std::sqrt( norm2( vec ) );
    }

    template< typename T, size_t D >
    T norm2( const Vector< T, D >& vec )
    {
        T result = 0;

        for( size_t i = 0; i < D; ++i )
            result += vec[ i ] * vec[ i ];

        return result;
    }

    template< typename T1, typename T2, size_t D >
    decltype( T1() * T2() ) dot(
        const Vector< T1, D >& lhs,
        const Vector< T2, D >& rhs )
    {
        decltype( T1() * T2() ) result = 0;

        for( size_t i = 0; i < D; ++i )
            result += lhs[ i ] * rhs[ i ];

        return result;
    }
}
