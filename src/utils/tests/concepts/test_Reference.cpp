#include <catch2/catch.hpp>

#include <ttb/utils/concepts.hpp>


namespace
{
    template < ttb::Reference TType >
    bool isReference();

    template < typename TType >
    bool isReference();
}

TEST_CASE( "Test reference concept", "[utils][concepts]" )
{
    REQUIRE( isReference< int& >() );
    REQUIRE( isReference< int const& >() );
    REQUIRE( !isReference< int >() );
    REQUIRE( !isReference< int const >() );
    REQUIRE( !isReference< int* >() );
    REQUIRE( !isReference< int const* >() );
    REQUIRE( !isReference< int&& >() );
}

namespace
{
    template < ttb::Reference TType >
    inline bool isReference()
    {
        return true;
    }

    template < typename TType >
    inline bool isReference()
    {
        return false;
    }
}
