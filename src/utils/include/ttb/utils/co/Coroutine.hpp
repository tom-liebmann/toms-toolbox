#pragma once

#include <ttb/utils/co/std_namespace.hpp>

#include <fmt/core.h>

#include <iostream>
#include <optional>
#include <type_traits>
#include <utility>


namespace ttb::co
{
    template < typename TResult >
    class Coroutine
    {
    public:
        class PromiseBase;
        class Promise;

        using promise_type = Promise;
        using Handle = ::co::coroutine_handle< Promise >;

        Coroutine();

        explicit Coroutine( Handle handle );

        Coroutine( Coroutine const& rhs ) = delete;
        Coroutine( Coroutine&& rhs );

        Coroutine& operator=( Coroutine const& rhs ) = delete;
        Coroutine& operator=( Coroutine&& rhs );

        ~Coroutine();

        bool isFinished() const;

        void resume();

        constexpr bool await_ready() const;

        template < typename TPromise >
        void await_suspend( ::co::coroutine_handle< TPromise > handle );

        template < typename U = TResult,
                   typename = typename std::enable_if_t< !std::is_void_v< U > > >
        U&& await_resume();

        template < typename U = TResult,
                   typename = typename std::enable_if_t< std::is_void_v< U > > >
        U await_resume();

    private:
        Handle m_handle;
    };


    class CoroutinePromiseBase
    {
    public:
        virtual bool resume() = 0;
    };


    template < typename TResult >
    class Coroutine< TResult >::PromiseBase : public CoroutinePromiseBase
    {
    public:
        std::optional< TResult >& value();

        template < typename TValue >
        void return_value( TValue&& value );

    private:
        std::optional< TResult > m_value;
    };


    template <>
    class Coroutine< void >::PromiseBase : public CoroutinePromiseBase
    {
    public:
        void return_void();
    };


    template < typename TResult >
    class Coroutine< TResult >::Promise : public PromiseBase
    {
    public:
        Promise();

        ~Promise();

        void checkException();

        virtual bool resume() override;

        void subpromise( CoroutinePromiseBase& promise );

        auto initial_suspend();

        auto final_suspend() noexcept;

        void unhandled_exception();

        Coroutine get_return_object();

    private:
        std::exception_ptr m_exception;
        CoroutinePromiseBase* m_subpromise{ nullptr };
    };
}  // namespace ttb::co


namespace ttb::co
{
    template < typename TResult >
    inline Coroutine< TResult >::Coroutine() = default;

    template < typename TResult >
    inline Coroutine< TResult >::Coroutine( Handle handle ) : m_handle{ handle }
    {
    }

    template < typename TResult >
    inline Coroutine< TResult >::Coroutine( Coroutine&& rhs )
        : m_handle{ std::exchange( rhs.m_handle, Handle{} ) }
    {
    }

    template < typename TResult >
    inline Coroutine< TResult >::~Coroutine()
    {
        fmt::print( "Destroying Coroutine\n" );
        if( m_handle )
        {
            fmt::print( "Destroying handle\n" );
            m_handle.destroy();
            fmt::print( "Clearing handle\n" );
            m_handle = Handle{};
        }
        fmt::print( "Destruction done\n" );
    }

    template < typename TResult >
    inline Coroutine< TResult >& Coroutine< TResult >::operator=( Coroutine&& rhs )
    {
        m_handle = std::exchange( rhs.m_handle, Handle{} );
        return *this;
    }

    template < typename TResult >
    inline bool Coroutine< TResult >::isFinished() const
    {
        return !m_handle || m_handle.done();
    }

    template < typename TResult >
    inline void Coroutine< TResult >::resume()
    {
        m_handle.promise().resume();
    }

    template < typename TResult >
    inline constexpr bool Coroutine< TResult >::await_ready() const
    {
        fmt::print( "await ready {}\n", m_handle.done() );
        // If the coroutine is already done, we can skip the suspension.
        return m_handle.done();
    }

    template < typename TResult >
    template < typename TPromise >
    inline void Coroutine< TResult >::await_suspend( ::co::coroutine_handle< TPromise > handle )
    {
        fmt::print( "Await suspend\n" );
        handle.promise().subpromise( m_handle.promise() );
    }

    template < typename TResult >
    template < typename U, typename >
    U&& Coroutine< TResult >::await_resume()
    {
        fmt::print( "await_resume 1\n" );
        m_handle.promise().checkException();

        return std::move( m_handle.promise().value().value() );
    }

    template < typename TResult >
    template < typename U, typename >
    U Coroutine< TResult >::await_resume()
    {
        fmt::print( "await_resume 2\n" );
        m_handle.promise().checkException();
    }


    template < typename TResult >
    std::optional< TResult >& Coroutine< TResult >::PromiseBase::value()
    {
        return m_value;
    }

    template < typename TResult >
    template < typename TValue >
    void Coroutine< TResult >::PromiseBase::return_value( TValue&& value )
    {
        fmt::print( "return_value\n" );
        m_value = std::forward< TValue >( value );
    }


    inline void Coroutine< void >::PromiseBase::return_void()
    {
        fmt::print( "return_void\n" );
    }


    template < typename TResult >
    Coroutine< TResult >::Promise::Promise()
    {
    }

    template < typename TResult >
    Coroutine< TResult >::Promise::~Promise()
    {
        fmt::print( "Destroying Promise {}\n", reinterpret_cast< intptr_t >( this ) );
    }

    template < typename TResult >
    void Coroutine< TResult >::Promise::checkException()
    {
        if( m_exception )
        {
            std::rethrow_exception( m_exception );
        }
    }

    template < typename TResult >
    bool Coroutine< TResult >::Promise::resume()
    {
        fmt::print( "Resume\n" );
        auto handle = Handle::from_promise( *this );

        checkException();

        if( m_subpromise )
        {
            if( m_subpromise->resume() )
            {
                return true;
            }
            else
            {
                m_subpromise = nullptr;
            }
        }

        if( handle.done() )
        {
            return false;
        }

        handle.resume();

        checkException();

        return !handle.done();
    }

    template < typename TResult >
    inline void Coroutine< TResult >::Promise::subpromise( CoroutinePromiseBase& promise )
    {
        m_subpromise = &promise;
    }

    template < typename TResult >
    inline auto Coroutine< TResult >::Promise::initial_suspend()
    {
        return ::co::suspend_never{};
    }

    template < typename TResult >
    inline auto Coroutine< TResult >::Promise::final_suspend() noexcept
    {
        fmt::print( "Final suspend\n" );
        return ::co::suspend_always{};
    }

    template < typename TResult >
    inline void Coroutine< TResult >::Promise::unhandled_exception()
    {
        fmt::print( "Got exception\n" );
        throw;
        // m_exception = std::current_exception();
    }

    template < typename TResult >
    inline auto Coroutine< TResult >::Promise::get_return_object() -> Coroutine
    {
        fmt::print( "get_return_object\n" );
        return Coroutine{ Handle::from_promise( *this ) };
    }
}  // namespace ttb::co
