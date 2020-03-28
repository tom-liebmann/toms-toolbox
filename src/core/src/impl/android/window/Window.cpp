#include <ttb/core/window/Window.hpp>
#include <ttb/core/window/WindowEvents.hpp>

#include <ttb/core/State.hpp>


namespace
{
    std::shared_ptr< ttb::Window >& instance();
}


namespace ttb
{
    class Window::Impl
    {
    };


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

    Window::EventOutput& Window::eventOutput()
    {
        return m_eventOutput;
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

    bool Window::use()
    {
        return true;
    }

    bool Window::unuse()
    {
        return true;
    }

    void Window::init( std::string const& title, uint16_t width, uint16_t height )
    {
        ::instance().reset(
            new Window( title, Mode{ width, height }, std::make_unique< Window::Impl >() ) );
    }

    std::shared_ptr< Window > Window::instance()
    {
        return ::instance();
    }

    Window::Window( std::string const& title, Mode const& mode, std::unique_ptr< Impl > impl )
        : m_impl{ std::move( impl ) }, m_mode{ mode }, m_title{ title }
    {
    }
}


namespace
{
    std::shared_ptr< ttb::Window >& instance()
    {
        static std::shared_ptr< ttb::Window > window;
        return window;
    }
}
