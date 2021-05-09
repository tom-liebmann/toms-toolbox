#include <ttb/core/window/Window.hpp>

#include "WindowImpl.hpp"

#include <cassert>


namespace ttb
{
    void Window::init( std::string_view title, WindowRequest const& request )
    {
        priv::WindowImpl::init( title, request );
    }

    Window& Window::instance()
    {
        auto& window = priv::WindowImpl::instance();

#ifndef NDEBUG
        assert( nullptr != window.get() );
#endif

        return *window;
    }
}
