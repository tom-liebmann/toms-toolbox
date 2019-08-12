#include <ttb/core/State.hpp>
#include <ttb/core/window/Window.hpp>


namespace ttb
{
    Window::Window( std::string const& title, Window::Mode const& mode )
        : m_mode( mode ), m_title( title )
    {
    }

    Window::~Window() = default;

    void Window::mode( Mode const& mode )
    {
        m_mode = mode;
    }

    Window::Mode const& Window::mode() const
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
        state.pushViewport( Viewport(
            0, 0, static_cast< GLsizei >( width() ), static_cast< GLsizei >( height() ) ) );
    }

    void Window::end( State& state ) const
    {
        state.popViewport();
    }


    Window::Mode::Mode( uint16_t width, uint16_t height, Flag flags )
        : m_width( width ), m_height( height ), m_flags( flags )
    {
    }

    uint16_t Window::Mode::width() const
    {
        return m_width;
    }

    uint16_t Window::Mode::height() const
    {
        return m_height;
    }

    bool Window::Mode::flag( Flag value ) const
    {
        return static_cast< uint32_t >( m_flags ) & static_cast< uint32_t >( value );
    }

    Window::Flag Window::Mode::flags() const
    {
        return m_flags;
    }


    Window::Flag operator|( Window::Flag lhs, Window::Flag rhs )
    {
        return static_cast< Window::Flag >( static_cast< uint32_t >( lhs ) |
                                            static_cast< uint32_t >( rhs ) );
    }
}
