#include <ttb/core/State.hpp>
#include <ttb/core/Viewport.hpp>
#include <ttb/core/window/Window.hpp>
#include <ttb/core/window/WindowEvents.hpp>

#include <SDL2/SDL.h>

#include <cassert>
#include <set>


namespace ttb
{
    namespace
    {
        class WindowImpl : public Window
        {
        public:
            static void init( std::string_view title, WindowRequest const& request );

            static std::unique_ptr< WindowImpl >& instance();

            ~WindowImpl();

            virtual void pollEvents() override;
            virtual void begin( State& state ) const override;
            virtual void end( State& state ) const override;
            virtual bool use() override;
            virtual bool unuse() override;

        private:
            WindowImpl( std::string_view title, Size const& size, WindowFlag flags );

            std::set< int > m_activePointers;

            SDL_Window* m_handle;
            SDL_GLContext m_context;
        };


        int convertPointerId( uint8_t pointerId );
    }
}


namespace ttb
{
    void Window::init( std::string_view title, WindowRequest const& request )
    {
        WindowImpl::init( title, request );
    }

    Window& Window::instance()
    {
        auto& window = WindowImpl::instance();

#ifndef NDEBUG
        assert( nullptr != window.get() );
#endif

        return *window;
    }
}


namespace ttb
{
    namespace
    {
        void WindowImpl::init( std::string_view title, WindowRequest const& request )
        {
            instance().reset( new WindowImpl{ title, request.size(), request.flags() } );
        }

        std::unique_ptr< WindowImpl >& WindowImpl::instance()
        {
            static std::unique_ptr< WindowImpl > s_instance;
            return s_instance;
        }

        WindowImpl::~WindowImpl()
        {
            SDL_GL_DeleteContext( m_context );
            SDL_DestroyWindow( m_handle );

            SDL_Quit();
        }

        void WindowImpl::pollEvents()
        {
            SDL_Event event;
            while( SDL_PollEvent( &event ) )
            {
                switch( event.type )
                {
                    case SDL_WINDOWEVENT:
                    {
                        switch( event.window.event )
                        {
                            case SDL_WINDOWEVENT_RESIZED:
                            {
                                m_size( 0 ) = event.window.data1;
                                m_size( 1 ) = event.window.data2;

                                auto const event = events::WindowResize{ *this };
                                pushEvent( event );

                                break;
                            }
                        }
                        break;
                    }

                    case SDL_KEYDOWN:
                    {
                        pushEvent( events::Key{ static_cast< uint32_t >( event.key.keysym.sym ),
                                                events::Key::Action::DOWN } );
                        break;
                    }

                    case SDL_KEYUP:
                    {
                        pushEvent( events::Key{ static_cast< uint32_t >( event.key.keysym.sym ),
                                                events::Key::Action::UP } );
                        break;
                    }

                    case SDL_MOUSEBUTTONDOWN:
                    {
                        auto const pointerId = convertPointerId( event.button.button );

                        m_activePointers.insert( pointerId );

                        pushEvent( events::PointerDown{ events::PointerType::MOUSE,
                                                        pointerId,
                                                        static_cast< double >( event.button.x ),
                                                        static_cast< double >( event.button.y ) } );
                        break;
                    }

                    case SDL_MOUSEBUTTONUP:
                    {
                        auto const pointerId = convertPointerId( event.button.button );

                        m_activePointers.erase( pointerId );

                        pushEvent( events::PointerUp{ pointerId,
                                                      static_cast< double >( event.button.x ),
                                                      static_cast< double >( event.button.y ) } );
                        break;
                    }

                    case SDL_MOUSEMOTION:
                    {
                        for( auto const pointerId : m_activePointers )
                        {
                            pushEvent(
                                events::PointerMove( pointerId, event.motion.x, event.motion.y ) );
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
        }

        void WindowImpl::begin( State& state ) const
        {
            auto const viewport = Viewport{
                0,
                0,
                static_cast< GLsizei >( m_size( 0 ) ),
                static_cast< GLsizei >( m_size( 1 ) ),
            };

            state.pushViewport( viewport );
        }

        void WindowImpl::end( State& state ) const
        {
            state.popViewport();

            SDL_GL_SwapWindow( m_handle );
        }

        bool WindowImpl::use()
        {
            return true;
        }

        bool WindowImpl::unuse()
        {
            return true;
        }

        WindowImpl::WindowImpl( std::string_view title, Size const& size, WindowFlag flags )
            : Window{ title, size, flags }
        {
            SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 0 );  // disable Antialiasing

            if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
            {
                throw std::runtime_error( "Failed to initialize SDL" );
            }

            if( GLEW_OK != glewInit() )
            {
                throw std::runtime_error( "Failed to initialize GLEW" );
            }

            m_handle = SDL_CreateWindow( m_title.c_str(),
                                         SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         m_size( 0 ),
                                         m_size( 1 ),
                                         SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );

            if( !m_handle )
            {
                throw std::runtime_error( "Failed to create window" );
            }

            int width;
            int height;
            SDL_GetWindowSize( m_handle, &width, &height );
            m_size( 0 ) = static_cast< uint16_t >( width );
            m_size( 1 ) = static_cast< uint16_t >( height );

            SDL_EventState( SDL_TEXTINPUT, SDL_DISABLE );
            SDL_EventState( SDL_KEYDOWN, SDL_DISABLE );
            SDL_EventState( SDL_KEYUP, SDL_DISABLE );

            SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );

            m_context = SDL_GL_CreateContext( m_handle );

            if( !m_context )
            {
                throw std::runtime_error( "Unable to create context: " +
                                          std::string( SDL_GetError() ) );
            }

            SDL_GL_SetSwapInterval( 1 );
        }


        int convertPointerId( uint8_t pointerId )
        {
            switch( pointerId )
            {
                case SDL_BUTTON_LEFT:
                    return 0;
                case SDL_BUTTON_RIGHT:
                    return 1;
                case SDL_BUTTON_MIDDLE:
                    return 2;
                default:
                    return 3;
            }
        }
    }
}
