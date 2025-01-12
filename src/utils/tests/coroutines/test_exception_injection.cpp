#include <catch2/catch.hpp>

#include <coroutine>
#include <ttb/utils/co/Coroutine.hpp>

namespace
{
    template < typename TResult >
    struct co_gethandle
    {
        using Coroutine = ttb::co::Coroutine< TResult >;
        using Handle = Coroutine::Handle;
        Handle m_handle;

        bool await_ready() const noexcept
        {
            return false;
        }

        bool await_suspend( Handle handle ) noexcept
        {
            m_handle = handle;
            return false;
        }

        Coroutine await_resume() noexcept
        {
            return Coroutine{ m_handle, false };
        }
    };
}


TEST_CASE( "Get handle inside coroutine", "[utils][coroutine]" )
{
    ttb::co::Coroutine< void > handle1;

    auto const coroutine = [ & ]() -> ttb::co::Coroutine< void >
    {
        handle1 = co_await co_gethandle< void >();

        co_await std::suspend_always{};

        co_return;
    };

    auto handle2 = coroutine();

    REQUIRE( !handle1 );
    REQUIRE( !handle2.isFinished() );

    handle2.resume();

    REQUIRE( handle1 );
    REQUIRE( !handle1.isFinished() );
    REQUIRE( !handle2.isFinished() );

    handle1.resume();

    REQUIRE( handle1.isFinished() );
    REQUIRE( handle2.isFinished() );
}

TEST_CASE( "Inject exception", "[utils][coroutine]" )
{
    auto endReached = false;

    auto const coroutine = [ & ]() -> ttb::co::Coroutine< void >
    {
        co_await std::suspend_always{};

        endReached = true;

        co_return;
    };

    auto handle = coroutine();

    REQUIRE( !handle.isFinished() );

    handle.resume();

    REQUIRE( !handle.isFinished() );

    handle.setException( std::runtime_error{ "Exception" } );

    REQUIRE_THROWS_WITH( handle.resume(), "Exception" );
    REQUIRE( !endReached );
    REQUIRE( handle.isFinished() );
}

TEST_CASE( "Exception caught", "[utils][coroutine]" )
{
    auto endReached = false;
    auto exceptionCaught = false;

    auto const coroutine = [ & ]() -> ttb::co::Coroutine< void >
    {
        try
        {
            co_await std::suspend_always{};
        }
        catch( std::exception& e )
        {
            exceptionCaught = true;
        }

        endReached = true;

        co_return;
    };

    auto handle = coroutine();

    REQUIRE( !handle.isFinished() );

    handle.resume();

    REQUIRE( !handle.isFinished() );

    handle.setException( std::runtime_error{ "Exception" } );

    handle.resume();

    REQUIRE( endReached );
    REQUIRE( exceptionCaught );
    REQUIRE( handle.isFinished() );
}

TEST_CASE( "Multiple layers", "[utils][coroutine]" )
{
    auto finished1 = false;
    auto const coroutine1 = [ & ]() -> ttb::co::Coroutine< void >
    {
        co_await std::suspend_always{};
        finished1 = true;
    };

    auto finished2 = false;
    auto const coroutine2 = [ & ]() -> ttb::co::Coroutine< void >
    {
        co_await coroutine1();
        finished2 = true;
    };

    auto handle = coroutine2();

    handle.resume();

    REQUIRE( !handle.isFinished() );
    REQUIRE( !finished1 );
    REQUIRE( !finished2 );

    handle.setException( std::runtime_error{ "Exception" } );

    REQUIRE_THROWS_WITH( handle.resume(), "Exception" );
    REQUIRE( handle.isFinished() );
    REQUIRE( !finished1 );
    REQUIRE( !finished2 );
}

TEST_CASE( "Multiple injection", "[utils][coroutine]" )
{
    auto finished = false;
    auto const coroutine = [ & ]() -> ttb::co::Coroutine< void >
    {
        try
        {
            co_await std::suspend_always{};
        }
        catch( std::exception& e )
        {
            // Do nothing
        }

        co_await std::suspend_always{};

        finished = true;
    };

    auto handle = coroutine();

    handle.resume();
    handle.setException( std::runtime_error( "Exception 1" ) );
    handle.resume();
    handle.setException( std::runtime_error( "Exception 2" ) );
    REQUIRE_THROWS_WITH( handle.resume(), "Exception 2" );
    REQUIRE( handle.isFinished() );
    REQUIRE( !finished );
}
