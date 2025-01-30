#pragma once

#include "Coroutine.hpp"


namespace ttb
{
    template < typename TResult >
    class GetHandle
    {
    public:
        using Coroutine = ttb::co::Coroutine< TResult >;
        using Handle = Coroutine::Handle;

        auto await_ready() const noexcept -> bool
        {
            return false;
        }

        auto await_suspend( Handle handle ) noexcept -> bool
        {
            m_handle = handle;
            return false;
        }

        auto await_resume() noexcept -> Coroutine
        {
            return Coroutine{ m_handle };
        }

    private:
        Handle m_handle;
    };
}
