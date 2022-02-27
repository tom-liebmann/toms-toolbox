#include <catch2/catch.hpp>

#include <ttb/math/Range.hpp>


TEST_CASE( "Range", "[math][range]" )
{
    auto range = ttb::Range< double, 2 >{};

    REQUIRE( range.empty() );

    range.expand( { 0.0, 0.0 } );
    range.expand( { 1.0, 1.0 } );

    REQUIRE( !range.empty() );

    range = {};

    REQUIRE( range.empty() );
}
