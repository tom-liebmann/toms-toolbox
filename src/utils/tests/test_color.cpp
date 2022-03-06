#include <catch2/catch.hpp>

#include <ttb/utils/Color.hpp>


TEST_CASE( "Color parsing", "[utils][color]" )
{
    REQUIRE( ttb::ColorRgb::createHexStr( "#f00" ).value() == ttb::ColorRgb::createI( 255, 0, 0 ) );
    REQUIRE( ttb::ColorRgb::createHexStr( "f00" ).value() == ttb::ColorRgb::createI( 255, 0, 0 ) );
    REQUIRE( ttb::ColorRgb::createHexStr( "#0a0" ).value() == ttb::ColorRgb::createI( 0, 170, 0 ) );
    REQUIRE( ttb::ColorRgb::createHexStr( "aabbcc" ).value() ==
             ttb::ColorRgb::createI( 170, 187, 204 ) );
    REQUIRE( ttb::ColorRgb::createHexStr( "#aabbcc" ).value() ==
             ttb::ColorRgb::createI( 170, 187, 204 ) );
}
