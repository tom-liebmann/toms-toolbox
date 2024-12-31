#pragma once

#include "ttb/utils/co/helper.hpp"
#include <exception>
#include <ttb/utils/co/std_namespace.hpp>

#include <fmt/core.h>

#include <optional>
#include <type_traits>
#include <utility>

#include <ttb/utils/co/Promise.hpp>


namespace ttb::co
{
    template < typename TResult >
    class Coroutine;

    template < typename TResult >
    auto operator==( Coroutine< TResult > const& lhs, Coroutine< TResult > const& rhs ) -> bool;

    template < typename TResult >
    class Coroutine
    {
    public:
        class PromiseBase;

        using promise_type = Promise< TResult >;
        using Handle = ::co::coroutine_handle< Promise< TResult > >;

        Coroutine();

        explicit Coroutine( Handle handle, bool destroy = true );

        Coroutine( Coroutine const& rhs ) = delete;
        Coroutine( Coroutine&& rhs );

        Coroutine& operator=( Coroutine const& rhs ) = delete;
        Coroutine& operator=( Coroutine&& rhs );

        ~Coroutine();

        bool isFinished() const;

        bool resume();

        void rethrowException() const;

        std::exception_ptr getException() const;

        template < typename TException >
        auto setException( TException&& e ) -> void;

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

        operator bool() const;

    private:
        Handle m_handle;
        bool m_destroy{ false };

        friend auto operator==
            < TResult >( Coroutine< TResult > const& lhs, Coroutine< TResult > const& rhs ) -> bool;
    };


    template < typename TResult >
    auto operator==( Coroutine< TResult > const& lhs, Coroutine< TResult > const& rhs ) -> bool;
}  // namespace ttb::co


namespace ttb::co
{
    template < typename TResult >
    inline Coroutine< TResult >::Coroutine() = default;

    template < typename TResult >
    inline Coroutine< TResult >::Coroutine( Handle handle, bool destroy )
        : m_handle{ handle }, m_destroy{ destroy }
    {
    }

    template < typename TResult >
    inline Coroutine< TResult >::Coroutine( Coroutine&& rhs )
        : m_handle{ std::exchange( rhs.m_handle, Handle{} ) }, m_destroy{ rhs.m_destroy }
    {
    }

    template < typename TResult >
    inline Coroutine< TResult >::~Coroutine()
    {
        if( m_handle )
        {
            if( m_destroy )
            {
                m_handle.destroy();
            }
            m_handle = Handle{};
        }
    }

    template < typename TResult >
    inline Coroutine< TResult >& Coroutine< TResult >::operator=( Coroutine&& rhs )
    {
        m_handle = std::exchange( rhs.m_handle, Handle{} );
        m_destroy = rhs.m_destroy;
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
    template < typename TException >
    auto Coroutine< TResult >::setException( TException&& e ) -> void
    {
        m_handle.promise().setException( std::forward< TException >( e ) );
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
    void Coroutine< TResult >::await_suspend( ::co::coroutine_handle< TPromise > handle )
    {
        if constexpr( helper::IsCoroutinePromise< TPromise >::value )
        {
            handle.promise().subpromise( m_handle.promise() );
        }
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
    Coroutine< TResult >::operator bool() const
    {
        return static_cast< bool >( m_handle );
    }



    template < typename TResult >
    auto operator==( Coroutine< TResult > const& lhs, Coroutine< TResult > const& rhs ) -> bool
    {
        return lhs.m_handle == rhs.m_handle;
    }
}  // namespace ttb::co
