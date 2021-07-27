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
}
