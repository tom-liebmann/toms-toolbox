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

        Coroutine() = default;

        explicit Coroutine( Handle handle );

        Coroutine( Coroutine const& rhs ) : m_handle{ rhs.m_handle }
        {
            m_handle.promise().increaseUse();
        }

        Coroutine( Coroutine&& rhs );

        ~Coroutine();

        auto operator=( Coroutine const& rhs ) -> Coroutine&
        {
            if( m_handle )
            {
                if( m_handle.promise().decreaseUse() )
                {
                    m_handle.destroy();
                }
            }

            m_handle = rhs.m_handle;
            return *this;
        }

        auto operator=( Coroutine&& rhs ) -> Coroutine&;


        auto isFinished() const -> bool;

        auto resume() -> bool;

        void rethrowException();

        auto getException() const -> std::exception_ptr;

        template < typename TException >
        void setException( TException&& e );

        auto getResult() -> std::optional< TResult >&;

        constexpr auto await_ready() const -> bool;

        template < typename TPromise >
        void await_suspend( ::co::coroutine_handle< TPromise > handle );

        template < typename U = TResult,
                   typename = typename std::enable_if_t< !std::is_void_v< U > > >
        auto await_resume() -> U&&;

        template < typename U = TResult,
                   typename = typename std::enable_if_t< std::is_void_v< U > > >
        auto await_resume() -> U;

        operator bool() const;

    private:
        Handle m_handle;

        friend auto operator==
            < TResult >( Coroutine< TResult > const& lhs, Coroutine< TResult > const& rhs ) -> bool;
    };


    template < typename TResult >
    auto operator==( Coroutine< TResult > const& lhs, Coroutine< TResult > const& rhs ) -> bool;
}  // namespace ttb::co


namespace ttb::co
{
    template < typename TResult >
    inline Coroutine< TResult >::Coroutine( Handle handle ) : m_handle{ handle }
    {
        m_handle.promise().increaseUse();
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
            if( m_handle.promise().decreaseUse() )
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
        rethrowException();
        auto const result = m_handle.promise().resume();
        rethrowException();
        return result;
    }

    template < typename TResult >
    inline void Coroutine< TResult >::rethrowException()
    {
        if( auto exceptionPtr = m_handle.promise().getException() )
        {
            if( m_handle.promise().decreaseUse() )
            {
                m_handle.destroy();
            }
            m_handle = Handle{};
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
        m_handle.promise().setException(
            std::make_exception_ptr( std::forward< TException >( e ) ) );
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
