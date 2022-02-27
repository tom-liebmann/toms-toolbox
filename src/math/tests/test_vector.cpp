#include <catch2/catch.hpp>

#include <ttb/math/Vector.hpp>
#include <ttb/math/vector_operations.hpp>


TEST_CASE( "Vector construction", "[math][vector]" )
{
    SECTION( "Empty" )
    {
        auto const vec = ttb::Vector< int, 2 >();
    }

    SECTION( "Initializer list" )
    {
        auto const vec = ttb::Vector< int, 2 >( { 1, 2 } );

        REQUIRE( 1 == vec( 0 ) );
        REQUIRE( 2 == vec( 1 ) );
    }

    SECTION( "Uniform initialization" )
    {
        auto const vec = ttb::Vector< int, 3 >{ 3, 4, 5 };

        REQUIRE( 3 == vec( 0 ) );
        REQUIRE( 4 == vec( 1 ) );
        REQUIRE( 5 == vec( 2 ) );
    }

    SECTION( "Type deduction" )
    {
        auto const vec = ttb::Vector{ 6, 7, 8 };

        REQUIRE( 3 == vec.size );
        REQUIRE( 6 == vec( 0 ) );
        REQUIRE( 7 == vec( 1 ) );
        REQUIRE( 8 == vec( 2 ) );
    }
}

TEST_CASE( "Vector assignment", "[math][vector]" )
{
    SECTION( "Copy" )
    {
        auto const vec1 = ttb::Vector{ 1, 2 };
        auto vec2 = ttb::Vector{ 3, 4 };

        // cppcheck-suppress redundantInitialization
        vec2 = vec1;

        REQUIRE( 1 == vec2( 0 ) );
        REQUIRE( 2 == vec2( 1 ) );
    }

    SECTION( "Implicit initializer list" )
    {
        auto vec = ttb::Vector{ 1, 2 };

        // cppcheck-suppress redundantInitialization
        vec = { 3, 4 };

        REQUIRE( 3 == vec( 0 ) );
        REQUIRE( 4 == vec( 1 ) );
    }

    SECTION( "Explicit single value" )
    {
        auto vec = ttb::Vector{ 1 };

        // cppcheck-suppress redundantInitialization
        vec = ttb::Vector{ 5 };

        REQUIRE( 5 == vec( 0 ) );
    }
}
