#include <ttb/core/State.hpp>
#include <ttb/core/window/Window.hpp>
#include <ttb/utils/dataIO.hpp>

#include <SDL2/SDL.h>


namespace ttb
{
    class Window::Impl
    {
    public:
        Impl( std::string const& title, WindowMode const& mode );

        ~Impl();

        std::shared_ptr< Provider< SlotType::ACTIVE, Event const& > > eventOutput() const;

        void update();

    private:
        static size_t s_windowCount;

        std::shared_ptr< SimpleProvider< SlotType::ACTIVE, Event const& > > m_eventOutput;

        SDL_Window* m_handle;
        SDL_GLContext m_context;
    };
}


namespace ttb
{
    Window::Window( std::string const& title, WindowMode const& mode )
        : m_title( title ), m_mode( mode ), m_impl( std::make_unique< Impl >( title, mode ) )
    {
    }

    Window::~Window()
    {
    }

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

    std::shared_ptr< Provider< SlotType::ACTIVE, Event const& > > Window::eventOutput() const
    {
        return m_impl->eventOutput();
    }

    void Window::update()
    {
        m_impl->update();
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


namespace ttb
{
    size_t Window::Impl::s_windowCount = 0;

    Window::Impl::Impl( std::string const& title, WindowMode const& mode )
    {
        if( s_windowCount == 0 )
        {
            SDL_Init( SDL_INIT_VIDEO );
            glewInit();
        }

        m_handle = SDL_CreateWindow( title.c_str(),
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     mode.width(),
                                     mode.height(),
                                     SDL_WINDOW_OPENGL |
                                         ( mode.fullscreen() ? SDL_WINDOW_FULLSCREEN : 0 ) );

        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );

        m_context = SDL_GL_CreateContext( m_handle );

        m_eventOutput = std::make_shared< SimpleProvider< SlotType::ACTIVE, Event const& > >();

        ++s_windowCount;
    }

    Window::Impl::~Impl()
    {
        SDL_GL_DeleteContext( m_context );
        SDL_DestroyWindow( m_handle );

        --s_windowCount;

        if( s_windowCount == 0 )
        {
            SDL_Quit();
        }
    }

    std::shared_ptr< Provider< SlotType::ACTIVE, Event const& > > Window::Impl::eventOutput() const
    {
        return m_eventOutput;
    }

    void Window::Impl::update()
    {
        SDL_GL_SwapWindow( m_handle );
    }
}
