#include <catch2/catch.hpp>

#include <ttb/math/trigonometry.hpp>
#include <ttb/math/utils.hpp>


TEST_CASE( "Factorial", "[math][utils]" )
{
    REQUIRE( ttb::factorial( 0 ) == 1 );
    REQUIRE( ttb::factorial( 1 ) == 1 );
    REQUIRE( ttb::factorial( 2 ) == 2 );
    REQUIRE( ttb::factorial( 3 ) == 6 );
    REQUIRE( ttb::factorial( 4 ) == 24 );
}


TEST_CASE( "Trigonometry", "[math][utils]" )
{
    SECTION( "Nearest angle (fixed cases)" )
    {
        REQUIRE( ttb::nearestAngle( 0.0, 0.0 ) == Approx( 0.0 ) );
        REQUIRE( ttb::nearestAngle( 0.0, 2.0 * ttb::PI< double > ) ==
                 Approx( 2.0 * ttb::PI< double > ) );

        REQUIRE( ttb::nearestAngle( 0.0, ttb::PI< double > - 1e-7 ) == Approx( 0.0 ) );
        REQUIRE( ttb::nearestAngle( 0.0, ttb::PI< double > + 1e-7 ) ==
                 Approx( 2.0 * ttb::PI< double > ) );
    }

    SECTION( "Nearest angle (generated)" )
    {
        auto const angle = GENERATE( range( 0.0, 20.0 * ttb::PI< double >, 0.3 ) );
        auto const ref = GENERATE( range( 0.0, 20.0 * ttb::PI< double >, 0.3 ) );
        REQUIRE( std::abs( ttb::nearestAngle( angle, ref ) - ref ) <= ttb::PI< double > );
    }
}
