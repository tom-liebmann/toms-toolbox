#include "WindowImpl.hpp"

#include <ttb/core/State.hpp>
#include <ttb/core/Viewport.hpp>
#include <ttb/core/window/WindowEvents.hpp>


namespace ttb
{
    WindowMode const& Window::Impl::mode() const
    {
        return m_mode;
    }

    std::string const& Window::Impl::title() const
    {
        return m_title;
    }

    void Window::Impl::window( Window& window )
    {
        m_window = &window;
    }

    void Window::Impl::eventCallback( EventCallback callback )
    {
        m_eventCallback = std::move( callback );
    }

    void Window::Impl::resize( uint16_t width, uint16_t height )
    {
        m_mode = WindowMode{ width, height, mode().flags() };

        if( m_eventCallback )
        {
            auto const event = ttb::events::WindowResize{ *m_window };
            m_eventCallback( event );
        }
    }

    void Window::Impl::begin( State& state )
    {
        auto const viewport = Viewport{
            0,
            0,
            static_cast< GLsizei >( mode().width() ),
            static_cast< GLsizei >( mode().height() ),
        };

        state.pushViewport( viewport );
    }

    void Window::Impl::end( State& state )
    {
        state.popViewport();
    }

    void Window::Impl::pushEvent( Event const& event )
    {
        if( m_eventCallback )
        {
            m_eventCallback( event );
        }
    }

    Window::Impl::Impl( std::string title, WindowMode const& mode )
        : m_mode{ mode }, m_title{ std::move( title ) }
    {
    }
}
