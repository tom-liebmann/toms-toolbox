#include "WindowImpl.hpp"

#include <ttb/core/State.hpp>
#include <ttb/core/Viewport.hpp>
#include <ttb/core/window/WindowCreator.hpp>
#include <ttb/core/window/WindowEvents.hpp>

#include <GL/glew.h>

#include <emscripten.h>


namespace ttb
{
    Window::Impl::~Impl()
    {
        SDL_GL_DeleteContext( m_context );
        SDL_DestroyWindow( m_handle );

        SDL_Quit();
    }

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

        SDL_GL_SwapWindow( m_handle );
    }

    void Window::Impl::pushEvent( Event const& event )
    {
        if( m_eventCallback )
        {
            m_eventCallback( event );
        }
    }

    void Window::Impl::update()
    {
        SDL_Event event;
        while( SDL_PollEvent( &event ) )
        {
            switch( event.type )
            {
                default:
                    break;
            }
        }
    }

    Window::Impl::Impl( std::string title, WindowMode const& mode )
        : m_mode{ mode }, m_title{ std::move( title ) }
    {
        std::cout << "debug1\n";
        SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 0 );  // disable Antialiasing
        std::cout << "debug2\n";

        if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
        {
            throw std::runtime_error( "Failed to initialize SDL" );
        }
        std::cout << "debug3\n";

        if( GLEW_OK != glewInit() )
        {
            throw std::runtime_error( "Failed to initialize GLEW" );
        }
        std::cout << "debug3 " << m_mode.width() << " " << m_mode.height() << " " << m_title
                  << "\n";

        m_handle = SDL_CreateWindow( m_title.c_str(),
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     m_mode.width(),
                                     m_mode.height(),
                                     SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );

        std::cout << "debug3\n";
        if( !m_handle )
        {
            throw std::runtime_error( "Failed to create window" );
        }

        SDL_EventState( SDL_TEXTINPUT, SDL_DISABLE );
        SDL_EventState( SDL_KEYDOWN, SDL_DISABLE );
        SDL_EventState( SDL_KEYUP, SDL_DISABLE );

        // SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

        m_context = SDL_GL_CreateContext( m_handle );

        if( !m_context )
        {
            throw std::runtime_error( "Unable to create context" );
        }

        SDL_GL_SetSwapInterval( 1 );
    }
}
