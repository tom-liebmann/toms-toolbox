#include <ttb/core/State.hpp>
#include <ttb/core/window/Window.hpp>


namespace ttb
{
    Window::Window( std::string const& title, WindowMode const& mode )
        : m_title( title ), m_mode( mode )
    {
    }

    Window::~Window() = default;

    void Window::mode( WindowMode const& mode )
    {
        m_mode = mode;
    }

    WindowMode const& Window::mode() const
    {
        return m_mode;
    }

    std::string const& Window::title() const
    {
        return m_title;
    }

    size_t Window::width() const
    {
        return m_mode.width();
    }

    size_t Window::height() const
    {
        return m_mode.height();
    }

    void Window::begin( State& state ) const
    {
        state.pushViewport( Viewport( 0, 0, width(), height() ) );
    }

    void Window::end( State& state ) const
    {
        state.popViewport();
    }
}
