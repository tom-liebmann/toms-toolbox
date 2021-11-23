#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <ttb/math/Bezier.hpp>


TEST_CASE( "Bezier", "[math][bezier]" )
{
    auto const bezier = ttb::Bezier< float, 2 >(
        { { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f } } );

    SECTION( "Point 1" )
    {
        auto const p = bezier( 0.5f );
        REQUIRE( p( 0 ) == Approx( 0.5f ) );
        REQUIRE( p( 1 ) == Approx( 0.75f ) );

        auto const d = bezier.derivative( 0.5f );
        REQUIRE( d( 0 ) == Approx( 1.5f ) );
        REQUIRE( d( 1 ) == Approx( 0.0f ) );
    }
}
