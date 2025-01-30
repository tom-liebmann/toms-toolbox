#include <catch2/catch.hpp>

#include <ttb/coroutine.hpp>


TEST_CASE( "Test copy constructor", "[utils][coroutine]" )
{
    auto const coroutine = [ & ]() -> ttb::co::Coroutine< void >
    {
        co_return;
    };

    auto handle1 = coroutine();
    auto handle2 = ttb::co::Coroutine< void >{ handle1 };

    SECTION( "Resuming via first handle" )
    {
        handle1.resume();
    }

    SECTION( "Resuming via second handle" )
    {
        handle2.resume();
    }

    REQUIRE( handle1.isFinished() );
    REQUIRE( handle2.isFinished() );
}
