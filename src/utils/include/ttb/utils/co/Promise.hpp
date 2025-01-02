#pragma once

#include <coroutine>
#include <ttb/utils/co/std_namespace.hpp>

#include <ttb/utils/co/ExceptionAwaitable.hpp>

#include <optional>

namespace ttb::co
{
    template < typename TResult >
    class Coroutine;

    class PromiseBase
    {
    public:
        virtual auto resume() -> bool = 0;

        virtual void setException( std::exception_ptr e ) noexcept = 0;
    };

    template < typename TResult >
    class TypedPromiseBase : public PromiseBase
    {
    public:
        auto value() noexcept -> std::optional< TResult >&
        {
            return m_value;
        }

        template < typename TValue >
        void return_value( TValue&& value ) noexcept
        {
            m_value = std::forward< TValue >( value );
        }

    private:
        std::optional< TResult > m_value;
    };

    template <>
    class TypedPromiseBase< void > : public PromiseBase
    {
    public:
        void return_void() noexcept
        {
        }
    };


    template < typename TResult >
    class Promise : public TypedPromiseBase< TResult >
    {
    public:
        using Handle = ::co::coroutine_handle< Promise >;

        virtual auto resume() noexcept -> bool override
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

        void subpromise( PromiseBase& promise ) noexcept
        {
            m_subpromise = &promise;
        }

        auto initial_suspend() noexcept
        {
            return ::co::suspend_always{};
        }

        auto final_suspend() noexcept
        {
            return ::co::suspend_always{};
        }

        auto get_return_object() noexcept
        {
            return Coroutine< TResult >{ Handle::from_promise( *this ) };
        }

        auto getException() const noexcept -> std::exception_ptr
        {
            return m_thrownException;
        }

        virtual void setException( std::exception_ptr e ) noexcept override
        {
            if( m_subpromise )
            {
                m_subpromise->setException( e );
            }
            else
            {
                m_injectedException = e;
            }
        }

        void unhandled_exception() noexcept
        {
            m_thrownException = std::current_exception();
        }

        template < typename TExpr >
        auto await_transform( TExpr&& expr ) noexcept
        {
            return ExceptionAwaitable{ std::forward< TExpr >( expr ), m_injectedException };
        }

    private:
        PromiseBase* m_subpromise{ nullptr };
        std::exception_ptr m_injectedException;
        std::exception_ptr m_thrownException;
    };
}
