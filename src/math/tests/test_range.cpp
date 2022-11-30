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

TEST_CASE( "Range Argument Deduction", "[math][range][deduction]" )
{
    SECTION( "Explicit construction" )
    {
        auto range = ttb::Range< float, 2 >{ { 0.0f, 0.0f }, { 1.0f, 1.0f } };

        REQUIRE( range.min( 0 ) == 0.0f );
        REQUIRE( range.min( 1 ) == 0.0f );
        REQUIRE( range.max( 0 ) == 1.0f );
        REQUIRE( range.max( 1 ) == 1.0f );
    }

    SECTION( "Full deduction" )
    {
        auto range = ttb::Range{ { 0.0f, 0.0f }, { 1.0f, 1.0f } };

        REQUIRE( range.min( 0 ) == 0.0f );
        REQUIRE( range.min( 1 ) == 0.0f );
        REQUIRE( range.max( 0 ) == 1.0f );
        REQUIRE( range.max( 1 ) == 1.0f );
    }
}
