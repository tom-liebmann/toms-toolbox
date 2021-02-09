#include <ttb/core/window/Window.hpp>


namespace ttb
{
    std::string_view Window::title() const
    {
        return m_title;
    }

    auto Window::size() const -> Size const&
    {
        return m_size;
    }

    bool Window::flag( WindowFlag value ) const
    {
        return static_cast< uint32_t >( m_flags ) & static_cast< uint32_t >( value );
    }

    void Window::eventCallback( EventCallback callback )
    {
        m_eventCallback = std::move( callback );
    }

    Window::Window( std::string_view title, Size const& size, WindowFlag flags )
        : m_title{ title }, m_size( size ), m_flags{ flags }
    {
    }

    size_t Window::width() const
    {
        return m_size( 0 );
    }

    size_t Window::height() const
    {
        return m_size( 1 );
    }
}
