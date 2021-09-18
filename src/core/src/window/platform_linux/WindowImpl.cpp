#include "WindowImpl.hpp"

#include <ttb/core/State.hpp>
#include <ttb/core/Viewport.hpp>
#include <ttb/core/window/WindowEvents.hpp>


namespace ttb
{
    namespace
    {
        inline int glfwBool( bool value );

        void callbackErrorGLFW( int error, char const* description );

        void callbackErrorOpenGL( GLenum source,
                                  GLenum type,
                                  GLuint id,
                                  GLenum severity,
                                  GLsizei length,
                                  GLchar const* message,
                                  void const* userParam );

        char const* convertErrorType( GLenum type );

        char const* convertErrorSeverity( GLenum severity );
    }
}


namespace ttb
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
        glfwDestroyWindow( m_handle );

        glfwTerminate();
    }

    GLFWwindow* WindowImpl::handle()
    {
        return m_handle;
    }

    void WindowImpl::pollEvents()
    {
        // glfwSwapBuffers( m_handle );
        glfwPollEvents();
    }

    void WindowImpl::begin( State::Data& /* data */ ) const
    {
        // Do nothing
    }

    void WindowImpl::suspend( State::Data& /* data */ ) const
    {
        // Do nothing
    }

    void WindowImpl::resume( State::Data& /* data */ ) const
    {
        // Do nothing
    }

    void WindowImpl::end( State::Data& /* data */ ) const
    {
        glfwSwapBuffers( m_handle );
    }

    bool WindowImpl::use()
    {
        glfwMakeContextCurrent( m_handle );
        return true;
    }

    bool WindowImpl::unuse()
    {
        glfwMakeContextCurrent( nullptr );
        return true;
    }

    WindowImpl::WindowImpl( std::string_view title, Size const& size, WindowFlag flags )
        : Window{ title, size, flags }
    {
        glfwInit();

        glfwSetErrorCallback( callbackErrorGLFW );

        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
        // glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

        glfwWindowHint( GLFW_SAMPLES, 8 );

#ifndef NDEBUG
        glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );
#endif

        GLFWmonitor* const monitor =
            flag( WindowFlag::FULLSCREEN ) ? glfwGetPrimaryMonitor() : nullptr;

        glfwWindowHint( GLFW_FLOATING, glfwBool( flag( WindowFlag::FLOATING ) ) );
        glfwWindowHint( GLFW_RESIZABLE, glfwBool( flag( WindowFlag::RESIZABLE ) ) );
        glfwWindowHint( GLFW_VISIBLE, glfwBool( !flag( WindowFlag::HIDDEN ) ) );

        m_handle = glfwCreateWindow( m_size( 0 ), m_size( 1 ), m_title.c_str(), monitor, nullptr );

        glfwSetWindowUserPointer( m_handle, this );

        glfwSetWindowCloseCallback( m_handle, onClose );
        glfwSetKeyCallback( m_handle, onKey );
        glfwSetMouseButtonCallback( m_handle, onMouseButton );
        glfwSetCursorPosCallback( m_handle, onMouseMove );
        glfwSetWindowSizeCallback( m_handle, onResize );
        glfwSetScrollCallback( m_handle, onScroll );

        // Initialize OpenGL context
        {
            glfwMakeContextCurrent( m_handle );

            glfwSwapInterval( 0 );

            // workaround for glew <= 1.13
            // see https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library
            glewExperimental = GL_TRUE;

            if( auto const error = glewInit(); error != GLEW_OK )
            {
                throw std::runtime_error(
                    "GLEW error: " +
                    std::string( reinterpret_cast< char const* >( glewGetErrorString( error ) ) ) );
            }

#ifndef NDEBUG
            if( glDebugMessageCallback )
            {
                glEnable( GL_DEBUG_OUTPUT );
                glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
                glDebugMessageCallback( callbackErrorOpenGL, nullptr );
            }
#endif

            glfwMakeContextCurrent( nullptr );
        }
    }

    void WindowImpl::onClose( GLFWwindow* window )
    {
        auto const wnd = reinterpret_cast< WindowImpl* >( glfwGetWindowUserPointer( window ) );

        auto const event = events::WindowClose{ *wnd };
        wnd->pushEvent( event );
    }

    void WindowImpl::onKey(
        GLFWwindow* window, int key, int /* scancode */, int action, int /* mods */ )
    {
        auto const wnd = reinterpret_cast< WindowImpl* >( glfwGetWindowUserPointer( window ) );

        auto const keyAction = [ & ]
        {
            switch( action )
            {
                case GLFW_PRESS:
                    return events::Key::Action::DOWN;

                case GLFW_RELEASE:
                    return events::Key::Action::UP;

                default:
                    return events::Key::Action::UNKNOWN;
            }
        }();

        auto const event = events::Key{ static_cast< uint32_t >( key ), keyAction };
        wnd->pushEvent( event );
    }

    void WindowImpl::onMouseButton( GLFWwindow* window, int button, int action, int /* mods */ )
    {
        auto const wnd = reinterpret_cast< WindowImpl* >( glfwGetWindowUserPointer( window ) );

        double x;
        double y;
        glfwGetCursorPos( window, &x, &y );

        auto const pointerId = [ & ]
        {
            switch( button )
            {
                case GLFW_MOUSE_BUTTON_LEFT:
                    return 0;

                case GLFW_MOUSE_BUTTON_RIGHT:
                    return 1;

                case GLFW_MOUSE_BUTTON_MIDDLE:
                    return 2;

                default:
                    return 3;
            }
        }();

        if( GLFW_PRESS == action )
        {
            wnd->m_activePointers.insert( pointerId );

            auto const event = events::PointerDown{ events::PointerType::MOUSE, pointerId, x, y };
            wnd->pushEvent( event );
        }
        else
        {
            wnd->m_activePointers.erase( pointerId );

            auto const event = events::PointerUp{ pointerId, x, y };
            wnd->pushEvent( event );
        }
    }

    void WindowImpl::onMouseMove( GLFWwindow* window, double x, double y )
    {
        auto const wnd = reinterpret_cast< WindowImpl* >( glfwGetWindowUserPointer( window ) );

        for( auto const pointerId : wnd->m_activePointers )
        {
            auto const event = events::PointerMove{ pointerId, x, y };
            wnd->pushEvent( event );
        }
    }

    void WindowImpl::onResize( GLFWwindow* window, int width, int height )
    {
        auto const wnd = reinterpret_cast< WindowImpl* >( glfwGetWindowUserPointer( window ) );

        wnd->m_size( 0 ) = static_cast< uint16_t >( width );
        wnd->m_size( 1 ) = static_cast< uint16_t >( height );

        auto const event = events::WindowResize{ *wnd };
        wnd->pushEvent( event );
    }

    void WindowImpl::onScroll( GLFWwindow* window, double xoffset, double yoffset )
    {
        auto const wnd = reinterpret_cast< WindowImpl* >( glfwGetWindowUserPointer( window ) );

        auto const event = events::Scroll{ xoffset, yoffset };
        wnd->pushEvent( event );
    }
}


namespace ttb
{
    namespace
    {
        int glfwBool( bool value )
        {
            return value ? GLFW_TRUE : GLFW_FALSE;
        }

        void callbackErrorGLFW( int error, char const* description )
        {
            std::cerr << "GLFW error (" << error << "): " << description << '\n';
        }

        void callbackErrorOpenGL( GLenum /* source */,
                                  GLenum type,
                                  GLuint /* id */,
                                  GLenum severity,
                                  GLsizei /* length */,
                                  GLchar const* message,
                                  void const* /* userParam */ )
        {
            if( severity == GL_DEBUG_SEVERITY_NOTIFICATION )
            {
                return;
            }

            std::cerr << "OpenGL error:\n"                                             //
                      << "    Type:     " << convertErrorType( type ) << '\n'          //
                      << "    Severity: " << convertErrorSeverity( severity ) << '\n'  //
                      << "    Message:  " << message << '\n';

            if( severity == GL_DEBUG_SEVERITY_HIGH )
            {
                throw std::runtime_error( "OpenGL error" );
            }
        }

        char const* convertErrorType( GLenum type )
        {
            switch( type )
            {
                case GL_DEBUG_TYPE_ERROR:
                    return "ERROR";
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                    return "DEPRECATED_BEHAVIOR";
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                    return "UNDEFINED_BEHAVIOR";
                case GL_DEBUG_TYPE_PORTABILITY:
                    return "PORTABILITY";
                case GL_DEBUG_TYPE_PERFORMANCE:
                    return "PERFORMANCE";
                case GL_DEBUG_TYPE_MARKER:
                    return "MARKER";
                case GL_DEBUG_TYPE_PUSH_GROUP:
                    return "PUSH_GROUP";
                case GL_DEBUG_TYPE_POP_GROUP:
                    return "POP_GROUP";
                case GL_DEBUG_TYPE_OTHER:
                    return "OTHER";
                default:
                    return "UNKNOWN";
            }
        }

        char const* convertErrorSeverity( GLenum severity )
        {
            switch( severity )
            {
                case GL_DEBUG_SEVERITY_LOW:
                    return "LOW";
                case GL_DEBUG_SEVERITY_MEDIUM:
                    return "MEDIUM";
                case GL_DEBUG_SEVERITY_HIGH:
                    return "HIGH";
                case GL_DEBUG_SEVERITY_NOTIFICATION:
                    return "NOTIFICATION";
                default:
                    return "UNKNOWN";
            }
        }
    }
}
