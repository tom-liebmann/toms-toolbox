#include <ttb/core/window/Window.hpp>

#include "WindowImpl.hpp"


namespace ttb
{
    Window::~Window() = default;

    WindowMode const& Window::mode() const
    {
        return m_impl->mode();
    }

    std::string const& Window::title() const
    {
        return m_impl->title();
    }

    void Window::eventCallback( EventCallback callback )
    {
        m_impl->eventCallback( std::move( callback ) );
    }

    void Window::resize( uint16_t width, uint16_t height )
    {
        m_impl->resize( width, height );
    }

    void Window::update()
    {
    }

    Window::Window( std::unique_ptr< Impl > impl ) : m_impl{ std::move( impl ) }
    {
        m_impl->window( *this );
    }

    size_t Window::width() const
    {
        return m_impl->mode().width();
    }

    size_t Window::height() const
    {
        return m_impl->mode().height();
    }

    void Window::begin( State& state ) const
    {
        m_impl->begin( state );
    }

    void Window::end( State& state ) const
    {
        m_impl->end( state );
    }

    bool Window::use()
    {
        return true;
    }

    bool Window::unuse()
    {
        return true;
    }
}
