#include <catch2/catch.hpp>

#include <ttb/utils/co/Coroutine.hpp>


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

    REQUIRE( coroutine.isFinished() );
    coroutine.resume();
    REQUIRE( coroutine.isFinished() );
}

TEST_CASE( "Lambda coroutine", "[utils][coroutine]" )
{
    auto const coroutine = []() -> ttb::co::Coroutine< void >
    {
        co_return;
    };

    auto handle = coroutine();

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
    handle.resume();
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
    handle.resume();
    REQUIRE( handle.isFinished() );
}

TEST_CASE( "Returned primitive", "[utils][coroutine]" )
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
    handle.resume();
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
    REQUIRE_THROWS_WITH( handle.resume(), "Exception" );
}

struct TestObject
{
public:
    explicit TestObject( int value ) : m_value{ value }
    {
    }

    ~TestObject()
    {
        fmt::print( "TestDestroy {}\n", m_value );
    }

private:
    int m_value;
};

TEST_CASE( "Exception transformation", "[utils][coroutine]" )
{
    auto const coroutine1 = []() -> ttb::co::Coroutine< void >
    {
        throw TestObject( 1 );
        // throw "Inner Exception";
    };

    auto const coroutine2 = [ &coroutine1 ]() -> ttb::co::Coroutine< void >
    {
        try
        {
            co_await coroutine1();
        }
        catch( TestObject& exception )
        {
            fmt::print( "Got inner exception\n" );
            throw TestObject( 2 );
            // throw "Outer Exception";
        }
    };

    // REQUIRE_THROWS_WITH( coroutine1(), "Inner Exception" );
    REQUIRE_THROWS_WITH( coroutine2(), "Outer Exception" );
    // auto handle = coroutine2();
    // REQUIRE_THROWS_WITH( handle.resume(), "Outer Exception" );
}
