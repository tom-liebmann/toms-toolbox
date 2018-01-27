#include <ttb/core/State.hpp>
#include <ttb/core/window/Window.hpp>
#include <ttb/core/window/WindowEvents.hpp>
#include <ttb/utils/dataIO.hpp>

#include "Window.hpp"


namespace ttb
{
    std::shared_ptr< Window > Window::create( std::string const& title, WindowMode const& mode )
    {
        return std::make_shared< ttb::emscripten::Window >( title, mode );
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
                                             ( mode.fullscreen() ? SDL_WINDOW_RESIZABLE : 0 ) );

            SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );

            m_context = SDL_GL_CreateContext( m_handle );

            m_eventOutput = std::make_unique< PushOutput< Event const& > >();

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

        PushOutput< Event const& >& Window::eventOutput()
        {
            return *m_eventOutput;
        }

        void Window::update()
        {
            SDL_GL_SwapWindow( m_handle );

            SDL_Event event;
            while( SDL_PollEvent( &event ) )
            {
                switch( event.type )
                {
                    case SDL_KEYDOWN:
                    {
                        m_eventOutput->push( ttb::events::Key( event.key.keysym.sym,
                                                               ttb::events::Key::Action::DOWN ) );
                        break;
                    }

                    case SDL_KEYUP:
                    {
                        m_eventOutput->push( ttb::events::Key( event.key.keysym.sym,
                                                               ttb::events::Key::Action::UP ) );
                        break;
                    }

                    case SDL_MOUSEBUTTONDOWN:
                    case SDL_MOUSEBUTTONUP:
                    {
                        auto button = [&event]() {
                            switch( event.button.button )
                            {
                                case SDL_BUTTON_LEFT:
                                    return ttb::events::MouseButton::Button::LEFT;
                                case SDL_BUTTON_RIGHT:
                                    return ttb::events::MouseButton::Button::RIGHT;
                                case SDL_BUTTON_MIDDLE:
                                    return ttb::events::MouseButton::Button::MIDDLE;
                                default:
                                    return ttb::events::MouseButton::Button::UNKNOWN;
                            }
                        }();

                        auto action = event.type == SDL_MOUSEBUTTONDOWN
                                          ? ttb::events::MouseButton::Action::DOWN
                                          : ttb::events::MouseButton::Action::UP;

                        m_eventOutput->push( ttb::events::MouseButton(
                            button, action, event.button.x, event.button.y ) );
                        break;
                    }

                    case SDL_MOUSEMOTION:
                    {
                        m_eventOutput->push(
                            ttb::events::MouseMove( event.motion.x, event.motion.y ) );
                        break;
                    }
                }
            }
        }
    }
}
