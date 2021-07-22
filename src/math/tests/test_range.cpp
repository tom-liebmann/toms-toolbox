#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <ttb/math/Range.hpp>


TEST_CASE( "Range", "[math][range]" )
{
    auto const range = ttb::Range< float, 2 >{ { 0.0f, 0.0f }, { 1.0f, 1.0f } };

    std::cout << range << '\n';
}
