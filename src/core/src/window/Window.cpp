#include <ttb/core/window/Window.hpp>


namespace ttb
{
    std::string_view Window::title() const
    {
        return m_title;
    }

    auto Window::viewport() const -> Viewport
    {
        return { { 0, 0 }, { m_size( 0 ), m_size( 1 ) } };
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

    void Window::pushEvent( Event const& event )
    {
        if( m_eventCallback )
        {
            m_eventCallback( event );
        }
    }
}
