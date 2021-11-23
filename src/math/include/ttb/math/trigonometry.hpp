#pragma once

#include "constants.hpp"

#include <cmath>
#include <cstdint>


// declarations
//=================================================================================================

namespace ttb
{
    template < typename TType >
    constexpr auto degToRad( TType degrees );

    /**
     * Finds the angle that is closest to \p ref and that corresponds to \p angle.
     *
     * This, e.g., is useful for interpolating angles in a way that preserves the periodicity.
     */
    template < typename TType >
    constexpr auto nearestAngle( TType angle, TType ref );
}



// definitions
//=================================================================================================

namespace ttb
{
    template < typename TType >
    constexpr auto degToRad( TType degrees )
    {
        return degrees / TType( 57.2957795130823208768 );
    }


    template < typename TType >
    constexpr auto nearestAngle( TType angle, TType ref )
    {
        // x = angle + k * 2 * PI
        // (ref - angle) / (2 * PI) - 1 < k <= (ref - angle) / (2 * PI)
        auto const k = std::floor( ( ref - angle ) / (TType( 2 ) * ttb::PI< TType >));
        auto const smallerAngle = angle + k * TType( 2 ) * ttb::PI< TType >;
        auto const biggerAngle = angle + ( k + 1 ) * TType( 2 ) * ttb::PI< TType >;

        return ref - smallerAngle < biggerAngle - ref ? smallerAngle : biggerAngle;
    }
}
