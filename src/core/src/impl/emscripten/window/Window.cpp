#include <ttb/core/State.hpp>
#include <ttb/core/window/Window.hpp>
#include <ttb/utils/dataIO.hpp>

#include "Window.hpp"


namespace ttb
{
    std::unique_ptr< Window > Window::create( std::string const& title, WindowMode const& mode )
    {
        return std::make_unique< ttb::emscripten::Window >( title, mode );
    }


    namespace emscripten
    {
        size_t Window::s_windowCount = 0;

        Window::Window( std::string const& title, WindowMode const& mode )
            : ttb::Window( title, mode )
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

        Window::~Window()
        {
            SDL_GL_DeleteContext( m_context );
            SDL_DestroyWindow( m_handle );

            --s_windowCount;

            if( s_windowCount == 0 )
            {
                SDL_Quit();
            }
        }

        std::shared_ptr< Provider< SlotType::ACTIVE, Event const& > > Window::eventOutput() const
        {
            return m_eventOutput;
        }

        void Window::update()
        {
            SDL_GL_SwapWindow( m_handle );
        }
    }
}
