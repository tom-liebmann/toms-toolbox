#include <catch2/catch.hpp>

#include <ttb/utils/co/Coroutine.hpp>
#include <ttb/utils/co/CoroutineRunner.hpp>


namespace
{
    ttb::co::Coroutine< void > simpleCoroutine()
    {
        co_return;
    }
}


TEST_CASE( "Simple coroutine", "[utils][coroutine]" )
{
    auto coroutine = simpleCoroutine();

    REQUIRE( !coroutine.isFinished() );
    REQUIRE( !coroutine.resume() );
    REQUIRE( coroutine.isFinished() );
}

TEST_CASE( "Lambda coroutine", "[utils][coroutine]" )
{
    auto const coroutine = []() -> ttb::co::Coroutine< void >
    {
        co_return;
    };

    auto handle = coroutine();

    REQUIRE( !handle.isFinished() );
    REQUIRE( !handle.resume() );
    REQUIRE( handle.isFinished() );
}

TEST_CASE( "Single wait", "[utils][coroutine]" )
{
    auto const coroutine = []() -> ttb::co::Coroutine< void >
    {
        co_await co::suspend_always{};
        co_return;
    };

    auto handle = coroutine();

    REQUIRE( !handle.isFinished() );
    REQUIRE( handle.resume() );
    REQUIRE( !handle.isFinished() );
    REQUIRE( !handle.resume() );
    REQUIRE( handle.isFinished() );
}

TEST_CASE( "Nested ready coroutines", "[utils][coroutine]" )
{
    auto const coroutine1 = []() -> ttb::co::Coroutine< void >
    {
        co_return;
    };

    auto const coroutine2 = [ &coroutine1 ]() -> ttb::co::Coroutine< void >
    {
        co_await coroutine1();
        co_return;
    };

    auto handle = coroutine2();

    REQUIRE( !handle.isFinished() );
    REQUIRE( !handle.resume() );
    REQUIRE( handle.isFinished() );
}

TEST_CASE( "Nested not ready coroutines", "[utils][coroutine]" )
{
    auto const coroutine1 = []() -> ttb::co::Coroutine< void >
    {
        co_await std::suspend_always{};
        co_return;
    };

    auto const coroutine2 = [ &coroutine1 ]() -> ttb::co::Coroutine< void >
    {
        co_await coroutine1();
        co_return;
    };

    auto handle = coroutine2();

    REQUIRE( !handle.isFinished() );
    REQUIRE( handle.resume() );
    REQUIRE( !handle.isFinished() );
    REQUIRE( !handle.resume() );
    REQUIRE( handle.isFinished() );
}

TEST_CASE( "Returned primitive", "[utils][coroutine]" )
{
    auto const coroutine = []( int input ) -> ttb::co::Coroutine< int >
    {
        co_return input;
    };

    auto handle = coroutine( 5 );

    REQUIRE( !handle.isFinished() );
    REQUIRE( !handle.getResult().has_value() );
    REQUIRE( !handle.resume() );
    REQUIRE( handle.getResult().has_value() );
    REQUIRE( handle.getResult().value() == 5 );
}

TEST_CASE( "Returned nested primitive", "[utils][coroutine]" )
{
    auto const coroutine1 = []( int input ) -> ttb::co::Coroutine< int >
    {
        co_return input;
    };

    auto const coroutine2 = [ &coroutine1 ]() -> ttb::co::Coroutine< void >
    {
        auto const value = co_await coroutine1( 5 );
        REQUIRE( value == 5 );
        co_return;
    };

    auto handle = coroutine2();

    REQUIRE( !handle.isFinished() );
    REQUIRE( !handle.resume() );
    REQUIRE( handle.isFinished() );
}

TEST_CASE( "Exception", "[utils][coroutine]" )
{
    auto const coroutine = []() -> ttb::co::Coroutine< void >
    {
        co_await co::suspend_always{};

        throw "Exception";
    };

    auto handle = coroutine();

    REQUIRE( !handle.isFinished() );
    REQUIRE( handle.resume() );
    REQUIRE( !handle.isFinished() );
    REQUIRE( !handle.resume() );
    REQUIRE( handle.isFinished() );
    REQUIRE( handle.getException() );
    REQUIRE_THROWS_WITH( handle.rethrowException(), "Exception" );
}

TEST_CASE( "Exception transformation", "[utils][coroutine]" )
{
    auto const coroutine1 = []() -> ttb::co::Coroutine< void >
    {
        throw "Inner Exception";
    };

    auto const coroutine2 = [ &coroutine1 ]() -> ttb::co::Coroutine< void >
    {
        try
        {
            co_await coroutine1();
        }
        catch( char const* exception )
        {
            throw "Outer Exception";
        }
    };

    REQUIRE_THROWS_WITH( coroutine1(), "Inner Exception" );

    auto handle = coroutine2();
    REQUIRE( !handle.isFinished() );
    REQUIRE( !handle.resume() );
    REQUIRE_THROWS_WITH( handle.rethrowException(), "Outer Exception" );
}

TEST_CASE( "Run single coroutine", "[utils][coroutine]" )
{
    auto const coroutine = []() -> ttb::co::Coroutine< void >
    {
        co_await co::suspend_always{};
        co_await co::suspend_always{};
    };

    auto runner = ttb::co::CoroutineRunner{};

    auto done = false;
    auto exception = std::exception_ptr{};

    runner.push(
        coroutine(),
        [ &done ]
        {
            done = true;
        },
        [ &exception ]( auto exceptionPtr )
        {
            exception = exceptionPtr;
        } );

    REQUIRE( !done );
    runner.run();
    REQUIRE( !done );
    runner.run();
    REQUIRE( done );
    REQUIRE( !exception );
}

TEST_CASE( "Run single coroutine with result", "[utils][coroutine]" )
{
    auto const coroutine = []() -> ttb::co::Coroutine< int >
    {
        co_return 3;
    };

    auto runner = ttb::co::CoroutineRunner{};

    auto result = std::optional< int >{};
    auto exception = std::exception_ptr{};

    runner.push(
        coroutine(),
        [ &result ]( int resultValue )
        {
            result = resultValue;
        },
        [ &exception ]( auto exceptionPtr )
        {
            exception = exceptionPtr;
        } );

    REQUIRE( result.has_value() );
    REQUIRE( result.value() == 3 );
    REQUIRE( !exception );
}
