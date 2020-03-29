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
    public:
        Impl( std::string const& title, Mode const& mode );

        std::string const& title() const;

        void mode( Mode const& mode );

        Mode const& mode() const;

        EventOutput& eventOutput();

    private:
        EventOutput m_eventOutput;
        std::string m_title;
        Mode m_mode;
    };


    Window::Mode const& Window::mode() const
    {
        return m_impl->mode();
    }

    std::string const& Window::title() const
    {
        return m_impl->title();
    }

    Window::EventOutput& Window::eventOutput()
    {
        return m_impl->eventOutput();
    }

    size_t Window::width() const
    {
        return mode().width();
    }

    size_t Window::height() const
    {
        return mode().height();
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
            new Window( std::make_unique< Window::Impl >( title, Mode{ width, height } ) ) );
    }

    std::shared_ptr< Window > Window::instance()
    {
        return ::instance();
    }

    Window::Window( std::unique_ptr< Impl > impl ) : m_impl{ std::move( impl ) }
    {
    }


    Window::Impl::Impl( std::string const& title, Mode const& mode )
        : m_title{ title }, m_mode{ mode }
    {
    }

    std::string const& Window::Impl::title() const
    {
        return m_title;
    }

    void Window::Impl::mode( Mode const& mode )
    {
        m_mode = mode;
    }

    Window::Mode const& Window::Impl::mode() const
    {
        return m_mode;
    }

    Window::EventOutput& Window::Impl::eventOutput()
    {
        return m_eventOutput;
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
