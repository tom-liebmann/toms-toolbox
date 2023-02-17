#pragma once

#include <exception>
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

        bool resume();

        void rethrowException() const;

        std::exception_ptr getException() const;

        std::optional< TResult >& getResult();

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

        virtual bool resume() override;

        void subpromise( CoroutinePromiseBase& promise );

        auto initial_suspend();

        auto final_suspend() noexcept;

        Coroutine get_return_object();

        std::exception_ptr getException() const;

        void unhandled_exception();

    private:
        CoroutinePromiseBase* m_subpromise{ nullptr };
        std::exception_ptr m_exception;
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
        if( m_handle )
        {
            m_handle.destroy();
            m_handle = Handle{};
        }
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
    inline bool Coroutine< TResult >::resume()
    {
        return m_handle.promise().resume();
    }

    template < typename TResult >
    inline void Coroutine< TResult >::rethrowException() const
    {
        if( auto exceptionPtr = m_handle.promise().getException() )
        {
            std::rethrow_exception( exceptionPtr );
        }
    }

    template < typename TResult >
    inline std::exception_ptr Coroutine< TResult >::getException() const
    {
        return m_handle.promise().getException();
    }

    template < typename TResult >
    std::optional< TResult >& Coroutine< TResult >::getResult()
    {
        return m_handle.promise().value();
    }

    template < typename TResult >
    inline constexpr bool Coroutine< TResult >::await_ready() const
    {
        // If the coroutine is already done, we can skip the suspension.
        return m_handle.done();
    }

    template < typename TResult >
    template < typename TPromise >
    inline void Coroutine< TResult >::await_suspend( ::co::coroutine_handle< TPromise > handle )
    {
        handle.promise().subpromise( m_handle.promise() );
    }

    template < typename TResult >
    template < typename U, typename >
    U&& Coroutine< TResult >::await_resume()
    {
        rethrowException();

        return std::move( m_handle.promise().value().value() );
    }

    template < typename TResult >
    template < typename U, typename >
    U Coroutine< TResult >::await_resume()
    {
        rethrowException();
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
        m_value = std::forward< TValue >( value );
    }


    inline void Coroutine< void >::PromiseBase::return_void()
    {
    }


    template < typename TResult >
    Coroutine< TResult >::Promise::Promise()
    {
    }

    template < typename TResult >
    Coroutine< TResult >::Promise::~Promise()
    {
    }

    template < typename TResult >
    bool Coroutine< TResult >::Promise::resume()
    {
        auto handle = Handle::from_promise( *this );

        do
        {
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

        } while( m_subpromise );

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
        return ::co::suspend_always{};
    }

    template < typename TResult >
    inline auto Coroutine< TResult >::Promise::final_suspend() noexcept
    {
        return ::co::suspend_always{};
    }

    template < typename TResult >
    inline auto Coroutine< TResult >::Promise::get_return_object() -> Coroutine
    {
        return Coroutine{ Handle::from_promise( *this ) };
    }

    template < typename TResult >
    inline std::exception_ptr Coroutine< TResult >::Promise::getException() const
    {
        return m_exception;
    }

    template < typename TResult >
    inline void Coroutine< TResult >::Promise::unhandled_exception()
    {
        m_exception = std::current_exception();
    }
}  // namespace ttb::co
