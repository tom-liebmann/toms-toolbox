#include <ttb/core/window/Window.hpp>

#include "WindowImpl.hpp"

#include <ttb/core/State.hpp>
#include <ttb/core/Viewport.hpp>
#include <ttb/core/gl.hpp>
#include <ttb/core/window/WindowEvents.hpp>

#include <GLFW/glfw3.h>

#include <cassert>
#include <set>


namespace ttb
{
    void Window::init( std::string_view title, WindowRequest const& request )
    {
        WindowImpl::init( title, request );
    }

    Window& Window::instance()
    {
        auto& window = WindowImpl::instance();

#ifndef NDEBUG
        assert( nullptr != window.get() );
#endif

        return *window;
    }
}
