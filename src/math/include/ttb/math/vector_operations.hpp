#pragma once

#include "Vector.hpp"

#include <cmath>
#include <iostream>


namespace ttb
{
    namespace impl
    {
        template < typename TType >
        constexpr TType HOMOGENIZE_THRESHOLD = static_cast< TType >( 1e-8 );
    }
}


// Declarations
//=============================================================================

namespace ttb
{
    template < typename TType1, typename TType2, size_t TDim >
    bool operator==( ttb::Vector< TType1, TDim > const& lhs,
                     ttb::Vector< TType2, TDim > const& rhs );

    template < typename TType1, typename TType2, size_t TDim >
    bool operator!=( ttb::Vector< TType1, TDim > const& lhs,
                     ttb::Vector< TType2, TDim > const& rhs );

    template < typename TType, size_t TDim >
    ttb::Vector< TType, TDim > operator-( ttb::Vector< TType, TDim > const& tensor );

    template < typename TType1, typename TType2, size_t TDim >
    auto operator+( ttb::Vector< TType1, TDim > const& lhs,
                    ttb::Vector< TType2, TDim > const& rhs );

    template < typename TType1, typename TType2, size_t TDim >
    auto operator-( ttb::Vector< TType1, TDim > const& lhs,
                    ttb::Vector< TType2, TDim > const& rhs );

    template < typename TType1, typename TType2, size_t TDim >
    ttb::Vector< TType1, TDim >& operator+=( ttb::Vector< TType1, TDim >& lhs,
                                             ttb::Vector< TType2, TDim > const& rhs );

    template < typename TType1, typename TType2, size_t TDim >
    ttb::Vector< TType1, TDim >& operator-=( ttb::Vector< TType1, TDim >& lhs,
                                             ttb::Vector< TType2, TDim > const& rhs );

    template < typename TType1,
               typename TType2,
               size_t TDim,
               typename Enabled = typename std::enable_if_t< std::is_arithmetic_v< TType2 > > >
    auto operator*( ttb::Vector< TType1, TDim > const& lhs, TType2 rhs );

    template < typename TType1,
               typename TType2,
               size_t TDim,
               typename Enabled = typename std::enable_if_t< std::is_arithmetic_v< TType1 > > >
    auto operator*( TType1 lhs, ttb::Vector< TType2, TDim > const& rhs );

    template < typename TType1, typename TType2, size_t TDim >
    auto operator/( ttb::Vector< TType1, TDim > const& lhs, TType2 rhs );

    template < typename TType1,
               typename TType2,
               size_t TDim,
               typename Enabled = typename std::enable_if_t< std::is_arithmetic_v< TType2 > > >
    ttb::Vector< TType1, TDim >& operator*=( ttb::Vector< TType1, TDim >& lhs, TType2 rhs );

    template < typename TType1, typename TType2, size_t TDim >
    ttb::Vector< TType1, TDim >& operator/=( ttb::Vector< TType1, TDim >& lhs, TType2 rhs );

    template < class TType, size_t TDim >
    std::ostream& operator<<( std::ostream& stream, ttb::Vector< TType, TDim > const& tensor );

    template < typename TType1, typename TType2, size_t TDim >
    auto min( Vector< TType1, TDim > const& lhs, Vector< TType2, TDim > const& rhs );

    template < typename TType1, typename TType2, size_t TDim >
    auto max( Vector< TType1, TDim > const& lhs, Vector< TType2, TDim > const& rhs );

    template < typename TType, size_t TDim >
    auto norm( Vector< TType, TDim > const& vec );

    template < typename TType, size_t TDim >
    auto norm2( Vector< TType, TDim > const& vec );

    template < typename TType, size_t TDim >
    auto normalize( Vector< TType, TDim > const& vec );

    template < typename TType1, typename TType2 >
    auto cross( Vector< TType1, 3 > const& lhs, Vector< TType2, 3 > const& rhs );

    template < typename TType1, typename TType2, size_t TDim >
    auto dot( Vector< TType1, TDim > const& lhs, Vector< TType2, TDim > const& rhs );

    template < typename TType, size_t TDim >
    Vector< TType, TDim + 1 > homogenize( Vector< TType, TDim > const& vec );

    template < typename TType, size_t TDim >
    Vector< TType, TDim - 1 > dehomogenize( Vector< TType, TDim > const& vec );
}


// Definitions
//=============================================================================

namespace ttb
{
    template < typename TType1, typename TType2, size_t TDim >
    bool operator==( ttb::Vector< TType1, TDim > const& lhs,
                     ttb::Vector< TType2, TDim > const& rhs )
    {
        for( size_t i = 0; i < lhs.size; ++i )
        {
            if( lhs[ i ] != rhs[ i ] )
            {
                return false;
            }
        }
        return true;
    }

    template < typename TType1, typename TType2, size_t TDim >
    bool operator!=( ttb::Vector< TType1, TDim > const& lhs,
                     ttb::Vector< TType2, TDim > const& rhs )
    {
        return !( lhs == rhs );
    }

    template < typename TType, size_t TDim >
    inline ttb::Vector< TType, TDim > operator-( ttb::Vector< TType, TDim > const& tensor )
    {
        ttb::Vector< TType, TDim > result;
        for( size_t i = 0; i < result.size; ++i )
        {
            result[ i ] = -tensor[ i ];
        }
        return result;
    }

    template < typename TType1, typename TType2, size_t TDim >
    inline auto operator+( ttb::Vector< TType1, TDim > const& lhs,
                           ttb::Vector< TType2, TDim > const& rhs )
    {
        using ResultType = decltype( std::declval< TType1 >() + std::declval< TType2 >() );

        ttb::Vector< ResultType, TDim > result;
        for( size_t i = 0; i < result.size; ++i )
        {
            result[ i ] = lhs[ i ] + rhs[ i ];
        }

        return result;
    }

    template < typename TType1, typename TType2, size_t TDim >
    inline auto operator-( ttb::Vector< TType1, TDim > const& lhs,
                           ttb::Vector< TType2, TDim > const& rhs )
    {
        using ResultType = decltype( std::declval< TType1 >() - std::declval< TType2 >() );

        ttb::Vector< ResultType, TDim > result;
        for( size_t i = 0; i < result.size; ++i )
        {
            result[ i ] = lhs[ i ] - rhs[ i ];
        }

        return result;
    }

    template < typename TType1, typename TType2, size_t TDim >
    ttb::Vector< TType1, TDim >& operator+=( ttb::Vector< TType1, TDim >& lhs,
                                             ttb::Vector< TType2, TDim > const& rhs )
    {
        for( size_t i = 0; i < lhs.size; ++i )
        {
            lhs[ i ] += rhs[ i ];
        }

        return lhs;
    }

    template < typename TType1, typename TType2, size_t TDim >
    ttb::Vector< TType1, TDim >& operator-=( ttb::Vector< TType1, TDim >& lhs,
                                             ttb::Vector< TType2, TDim > const& rhs )
    {
        for( size_t i = 0; i < lhs.size; ++i )
        {
            lhs[ i ] -= rhs[ i ];
        }

        return lhs;
    }

    template < typename TType1, typename TType2, size_t TDim, typename Enabled >
    auto operator*( ttb::Vector< TType1, TDim > const& lhs, TType2 rhs )
    {
        using ResultType = decltype( std::declval< TType1 >() * std::declval< TType2 >() );

        ttb::Vector< ResultType, TDim > result;
        for( size_t i = 0; i < lhs.size; ++i )
        {
            result[ i ] = lhs[ i ] * rhs;
        }

        return result;
    }

    template < typename TType1, typename TType2, size_t TDim, typename Enabled >
    auto operator*( TType1 lhs, ttb::Vector< TType2, TDim > const& rhs )
    {
        using ResultType = decltype( std::declval< TType1 >() * std::declval< TType2 >() );

        ttb::Vector< ResultType, TDim > result;
        for( size_t i = 0; i < rhs.size; ++i )
        {
            result[ i ] = lhs * rhs[ i ];
        }

        return result;
    }

    template < typename TType1, typename TType2, size_t TDim >
    auto operator/( ttb::Vector< TType1, TDim > const& lhs, TType2 rhs )
    {
        using ResultType = decltype( std::declval< TType1 >() / std::declval< TType2 >() );

        ttb::Vector< ResultType, TDim > result;
        for( size_t i = 0; i < lhs.size; ++i )
        {
            result[ i ] = lhs[ i ] / rhs;
        }

        return result;
    }

    template < typename TType1, typename TType2, size_t TDim, typename Enabled >
    ttb::Vector< TType1, TDim >& operator*=( ttb::Vector< TType1, TDim >& lhs, TType2 const& rhs )
    {
        for( size_t i = 0; i < lhs.size; ++i )
        {
            lhs[ i ] *= rhs;
        }

        return lhs;
    }

    template < typename TType1, typename TType2, size_t TDim >
    ttb::Vector< TType1, TDim >& operator/=( ttb::Vector< TType1, TDim >& lhs, TType2 const& rhs )
    {
        for( size_t i = 0; i < lhs.size; ++i )
        {
            lhs[ i ] /= rhs;
        }

        return lhs;
    }

    template < class TType, size_t TDim >
    std::ostream& operator<<( std::ostream& stream, ttb::Vector< TType, TDim > const& tensor )
    {
        stream << '(';
        for( size_t i = 0; i < tensor.size; ++i )
        {
            if( i != 0 )
            {
                stream << ", ";
            }
            stream << tensor[ i ];
        }
        stream << ')';

        return stream;
    }

    template < typename TType1, typename TType2, size_t TDim >
    auto min( Vector< TType1, TDim > const& lhs, Vector< TType2, TDim > const& rhs )
    {
        using std::min;

        using ResultType = decltype( min( std::declval< TType1 >(), std::declval< TType2 >() ) );

        Vector< ResultType, TDim > result;
        for( size_t i = 0; i < lhs.size; ++i )
        {
            result[ i ] = min( lhs[ i ], rhs[ i ] );
        }

        return result;
    }

    template < typename TType1, typename TType2, size_t TDim >
    auto max( Vector< TType1, TDim > const& lhs, Vector< TType2, TDim > const& rhs )
    {
        using std::max;

        using ResultType = decltype( max( std::declval< TType1 >(), std::declval< TType2 >() ) );

        Vector< ResultType, TDim > result;
        for( size_t i = 0; i < lhs.size; ++i )
        {
            result[ i ] = max( lhs[ i ], rhs[ i ] );
        }

        return result;
    }

    template < typename TType, size_t TDim >
    auto norm( Vector< TType, TDim > const& vec )
    {
        using std::sqrt;
        return sqrt( norm2( vec ) );
    }

    template < typename TType, size_t TDim >
    auto norm2( Vector< TType, TDim > const& vec )
    {
        TType result = 0;

        for( size_t d = 0; d < TDim; ++d )
        {
            result += vec[ d ] * vec[ d ];
        }

        return result;
    }

    template < typename TType, size_t TDim >
    auto normalize( Vector< TType, TDim > const& vec )
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

    template < typename TType1, typename TType2, size_t TDim >
    auto dot( Vector< TType1, TDim > const& lhs, Vector< TType2, TDim > const& rhs )
    {
        using ReturnType = decltype( std::declval< TType1 >() * std::declval< TType2 >() );

        ReturnType result = 0;

        for( size_t d = 0; d < TDim; ++d )
        {
            result += lhs[ d ] * rhs[ d ];
        }

        return result;
    }

    template < typename TType, size_t TDim >
    Vector< TType, TDim + 1 > homogenize( Vector< TType, TDim > const& vec )
    {
        Vector< TType, TDim + 1 > result;
        for( size_t i = 0; i < TDim; ++i )
        {
            result( i ) = vec( i );
        }

        result( TDim ) = 1;

        return result;
    }

    template < typename TType, size_t TDim >
    Vector< TType, TDim - 1 > dehomogenize( Vector< TType, TDim > const& vec )
    {
        static_assert( TDim > 0 );

        using std::abs;

        Vector< TType, TDim - 1 > result;
        for( size_t i = 0; i < TDim - 1; ++i )
        {
            result( i ) = vec( i ) / vec( TDim - 1 );
        }

        return result;
    }
}
