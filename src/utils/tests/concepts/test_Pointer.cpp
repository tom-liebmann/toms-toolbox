#include <catch2/catch.hpp>

#include <ttb/utils/concepts.hpp>


namespace
{
    template < ttb::Pointer TType >
    bool isPointer();

    template < typename TType >
    bool isPointer();
}

TEST_CASE( "Test pointer concept", "[utils][concepts]" )
{
    REQUIRE( isPointer< int* >() );
    REQUIRE( isPointer< int const* >() );
    REQUIRE( !isPointer< int >() );
    REQUIRE( !isPointer< int const >() );
    REQUIRE( !isPointer< int& >() );
    REQUIRE( !isPointer< int const& >() );
    REQUIRE( !isPointer< int&& >() );
}

namespace
{
    template < ttb::Pointer TType >
    inline bool isPointer()
    {
        return true;
    }

    template < typename TType >
    inline bool isPointer()
    {
        return false;
    }
}
