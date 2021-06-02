#pragma once


namespace ttb
{
    template < typename TType >
    constexpr TType abs( TType value );

    template < typename TType >
    constexpr TType fmod( TType numer, TType denom );
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
}
