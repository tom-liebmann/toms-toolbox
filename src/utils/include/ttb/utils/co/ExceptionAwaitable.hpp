#pragma once

#include <ttb/utils/co/std_namespace.hpp>

#include <exception>

namespace ttb::co
{
    template < typename Awaitable >
    class ExceptionAwaitable
    {
    public:
        ExceptionAwaitable( Awaitable awaitable, std::exception_ptr& exceptionPtr )
            : m_awaitable{ std::move( awaitable ) }, m_exceptionPtr{ exceptionPtr }
        {
        }

        auto await_ready() noexcept -> bool
        {
            return m_awaitable.await_ready();
        }

        template < typename TPromise >
        auto await_suspend( std::coroutine_handle< TPromise > h )
        {
            if( m_exceptionPtr )
            {
                std::rethrow_exception( m_exceptionPtr );
            }
            return m_awaitable.await_suspend( h );
        }

        auto await_resume()
        {
            if( m_exceptionPtr )
            {
                std::rethrow_exception( m_exceptionPtr );
            }
            return m_awaitable.await_resume();
        }

    private:
        Awaitable m_awaitable;
        std::exception_ptr& m_exceptionPtr;
    };
}
