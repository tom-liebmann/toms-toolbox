#pragma once

#include <algorithm>
#include <cmath>


namespace ttb
{
    template < typename TType >
    constexpr TType abs( TType value );

    template < typename TType >
    constexpr TType fmod( TType numer, TType denom );

    template < typename TType >
    constexpr auto clamp( TType value, TType lowerBound, TType upperBound );

    template < typename TType1, typename TType2, typename TType3 >
    constexpr auto mix( TType1 min, TType2 max, TType3 value );

    template < typename TType >
    constexpr auto smoothstep( TType value );

    template < typename TType >
    auto factorial( TType value );

    template < typename TType >
    auto binomial( TType n, TType k );
}


namespace ttb
{
    template < typename TType >
    inline constexpr TType abs( TType value )
    {
        return value >= 0 ? value : -value;
    }

    template < typename TType >
    inline constexpr TType fmod( TType numer, TType denom )
    {
        auto result = std::remainder( std::fabs( numer ), ( denom = std::fabs( denom ) ) );
        if( std::signbit( result ) )
        {
            result += denom;
        }
        return std::copysign( result, numer );
    }

    template < typename TType >
    inline constexpr auto clamp( TType value, TType minValue, TType maxValue )
    {
        using std::max;
        using std::min;
        return min( maxValue, max( minValue, value ) );
    }

    template < typename TType1, typename TType2, typename TType3 >
    inline constexpr auto mix( TType1 min, TType2 max, TType3 value )
    {
        return ( 1 - value ) * min + value * max;
    }

    template < typename TType >
    inline constexpr auto smoothstep( TType value )
    {
        auto const x = clamp( value, TType{ 0 }, TType{ 1 } );
        return x * x * ( TType{ 3 } - TType{ 2 } * x );
    }

    template < typename TType >
    auto factorial( TType value )
    {
        auto result = TType{ 1 };

        while( value > 1 )
        {
            result *= value;
            value -= 1;
        }

        return result;
    }

    template < typename TType >
    auto binomial( TType n, TType k )
    {
        return factorial( n ) / factorial( k ) / factorial( n - k );
    }
}
