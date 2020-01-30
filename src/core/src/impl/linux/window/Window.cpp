#include "Window.hpp"
#include <ttb/core/window/Window.hpp>
#include <ttb/core/window/WindowEvents.hpp>

#include <ttb/core/State.hpp>

#include <GLFW/glfw3.h>


#ifdef WIN32
#define STDCALL __attribute__( ( __stdcall__ ) )
#else
#define STDCALL
#endif

namespace
{
    void pushEvent( GLFWwindow* window, ttb::Event const& event );

    void callbackErrorGLFW( int error, char const* description );

    STDCALL void callbackErrorOpenGL( GLenum source,
                                      GLenum type,
                                      GLuint id,
                                      GLenum severity,
                                      GLsizei length,
                                      GLchar const* message,
                                      void const* userParam );

    void callbackWindowClose( GLFWwindow* window );

    void callbackKey( GLFWwindow* window, int key, int scancode, int action, int mods );

    void callbackMouseButton( GLFWwindow* window, int button, int action, int mods );

    void callbackMouseMove( GLFWwindow* window, double x, double y );

    void callbackWindowSize( GLFWwindow* window, int width, int height );

    void callbackScroll( GLFWwindow* window, double xoffset, double yoffset );

    std::string convertErrorType( GLenum type );

    std::string convertErrorSeverity( GLenum severity );
}


namespace ttb
{
    std::shared_ptr< Window > Window::create( std::string const& title, Window::Mode const& mode )
    {
        return std::make_shared< linux::Window >( title, mode );
    }

    std::shared_ptr< Window > Window::create( std::string const& title,
                                              Window::Mode const& mode,
                                              ttb::Window const& sharedWindow )
    {
        return std::make_shared< linux::Window >( title, mode, sharedWindow );
    }


    namespace linux
    {
        size_t Window::s_windowCount = 0;

        Window::Window( std::string const& title, Mode const& mode )
            : Window( title, mode, nullptr )
        {
        }

        Window::Window( std::string const& title,
                        Mode const& mode,
                        ttb::Window const& sharedWindow )
            : Window( title, mode, static_cast< Window const& >( sharedWindow ).m_handle )
        {
        }

        Window::Window( std::string const& title, Mode const& mode, GLFWwindow* sharedWindow )
            : ttb::Window( title, mode )
        {
            if( s_windowCount == 0 )
            {
                glfwInit();
            }

            glfwSetErrorCallback( callbackErrorGLFW );

            // prevent glfw to create a context < 3.3
            // glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
            // glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
            // glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
            // glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#ifndef NDEBUG
            std::cout << "glfw version: " << glfwGetVersionString() << std::endl;
            std::cout << "glew version: " << glewGetString( GLEW_VERSION ) << std::endl;

            glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );
#endif

            GLFWmonitor* monitor =
                mode.flag( Window::Flag::FULLSCREEN ) ? glfwGetPrimaryMonitor() : nullptr;

            glfwWindowHint( GLFW_FLOATING,
                            mode.flag( Window::Flag::FLOATING ) ? GL_TRUE : GL_FALSE );
            glfwWindowHint( GLFW_RESIZABLE,
                            mode.flag( Window::Flag::RESIZABLE ) ? GL_TRUE : GL_FALSE );

            if( mode.flag( Window::Flag::HIDDEN ) )
            {
                glfwWindowHint( GLFW_VISIBLE,
                                mode.flag( Window::Flag::HIDDEN ) ? GL_FALSE : GL_TRUE );
                m_handle =
                    glfwCreateWindow( mode.width(), mode.height(), "", nullptr, sharedWindow );
            }
            else
            {
                m_handle = glfwCreateWindow(
                    mode.width(), mode.height(), title.c_str(), monitor, sharedWindow );
            }

            glfwMakeContextCurrent( m_handle );

#ifndef NDEBUG
            std::cout << "Context: " << reinterpret_cast< intptr_t >( m_handle ) << '\n';
#endif

            glfwSwapInterval( 0 );

            if( s_windowCount == 0 )
            {
                // workaround for glew <= 1.13
                // see https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library
                glewExperimental = GL_TRUE;

                if( auto error = glewInit(); error != GLEW_OK )
                {
                    throw std::runtime_error( "GLEW error: " +
                                              std::string( reinterpret_cast< char const* >(
                                                  glewGetErrorString( error ) ) ) );
                }
            }

#ifndef NDEBUG
            std::cout << "context major version: "
                      << glfwGetWindowAttrib( m_handle, GLFW_CONTEXT_VERSION_MAJOR ) << std::endl;
            std::cout << "context minor version: "
                      << glfwGetWindowAttrib( m_handle, GLFW_CONTEXT_VERSION_MINOR ) << std::endl;

            std::cout << "Profile: ";

            switch( glfwGetWindowAttrib( m_handle, GLFW_OPENGL_PROFILE ) )
            {
                case GLFW_OPENGL_CORE_PROFILE:
                    std::cout << "core\n";
                    break;

                case GLFW_OPENGL_COMPAT_PROFILE:
                    std::cout << "compat\n";
                    break;

                case GLFW_OPENGL_ANY_PROFILE:
                    std::cout << "any\n";
                    break;
            }

            if( !glfwExtensionSupported( "GL_ARB_debug_output" ) )
            {
                std::cout << "Debugging extension not supported" << std::endl;
            }

            if( glDebugMessageCallback )
            {
                std::cout << "Debugging enabled" << std::endl;
                glEnable( GL_DEBUG_OUTPUT );
                glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
                glDebugMessageCallback( callbackErrorOpenGL, nullptr );
            }
            else
            {
                std::cout << "Debugging not available" << std::endl;
            }
#endif

            glfwSetWindowUserPointer( m_handle, this );

            glfwSetWindowCloseCallback( m_handle, callbackWindowClose );
            glfwSetKeyCallback( m_handle, callbackKey );
            glfwSetMouseButtonCallback( m_handle, callbackMouseButton );
            glfwSetCursorPosCallback( m_handle, callbackMouseMove );
            glfwSetWindowSizeCallback( m_handle, callbackWindowSize );
            glfwSetScrollCallback( m_handle, callbackScroll );

            ++s_windowCount;

            glfwMakeContextCurrent( nullptr );
        }

        Window::~Window()
        {
            glfwDestroyWindow( m_handle );

            --s_windowCount;

            if( s_windowCount == 0 )
            {
                glfwTerminate();
            }
        }

        Signal< void( Event const& ) >& Window::eventOutput()
        {
            return m_eventOutput;
        }

        void Window::update()
        {
            glfwSwapBuffers( m_handle );
            glfwPollEvents();
        }

        void Window::resize( uint16_t width, uint16_t height )
        {
            glfwSetWindowSize( m_handle, width, height );

            mode( ttb::Window::Mode( width, height, mode().flags() ) );

            m_eventOutput.call( ttb::events::WindowResize( *this ) );
        }

        bool Window::use()
        {
            glfwMakeContextCurrent( m_handle );
            return true;
        }

        bool Window::unuse()
        {
            glfwMakeContextCurrent( nullptr );
            return true;
        }
    }
}


namespace
{
    void pushEvent( GLFWwindow* window, ttb::Event const& event )
    {
        auto wnd = reinterpret_cast< ttb::linux::Window* >( glfwGetWindowUserPointer( window ) );

        wnd->eventOutput().call( event );
    }

    void callbackErrorGLFW( int error, char const* description )
    {
        std::cout << "GLFW error (" << error << "): " << description << std::endl;
    }

    STDCALL void callbackErrorOpenGL( GLenum /* source */,
                                      GLenum type,
                                      GLuint /* id */,
                                      GLenum severity,
                                      GLsizei /* length */,
                                      GLchar const* message,
                                      void const* /* userParam */ )
    {
        std::cout << "OpenGL error:" << std::endl;
        std::cout << "    Type: " << convertErrorType( type ) << std::endl;
        std::cout << "    Severity: " << convertErrorSeverity( severity ) << std::endl;
        std::cout << "    Message: " << message << std::endl;

        if( severity == GL_DEBUG_SEVERITY_HIGH )
        {
            throw std::runtime_error( "OpenGL error" );
        }
    }

    void callbackWindowClose( GLFWwindow* window )
    {
        auto wnd = reinterpret_cast< ttb::linux::Window* >( glfwGetWindowUserPointer( window ) );

        pushEvent( window, ttb::events::WindowClose( *wnd ) );
    }

    void callbackKey( GLFWwindow* window, int key, int /* scancode */, int action, int /* mods */ )
    {
        switch( action )
        {
            case GLFW_PRESS:
                pushEvent( window, ttb::events::Key( key, ttb::events::Key::Action::DOWN ) );
                break;

            case GLFW_RELEASE:
                pushEvent( window, ttb::events::Key( key, ttb::events::Key::Action::UP ) );
                break;
        }
    }

    void callbackMouseButton( GLFWwindow* window, int button, int action, int /* mods */ )
    {
        ttb::events::MouseButton::Button mouseButton;
        switch( button )
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                mouseButton = ttb::events::MouseButton::Button::LEFT;
                break;

            case GLFW_MOUSE_BUTTON_RIGHT:
                mouseButton = ttb::events::MouseButton::Button::RIGHT;
                break;

            case GLFW_MOUSE_BUTTON_MIDDLE:
                mouseButton = ttb::events::MouseButton::Button::MIDDLE;
                break;

            default:
                return;
        }

        double mouseX, mouseY;
        glfwGetCursorPos( window, &mouseX, &mouseY );

        pushEvent( window,
                   ttb::events::MouseButton( mouseButton,
                                             action == GLFW_PRESS
                                                 ? ttb::events::MouseButton::Action::DOWN
                                                 : ttb::events::MouseButton::Action::UP,
                                             mouseX,
                                             mouseY ) );
    }

    void callbackMouseMove( GLFWwindow* window, double x, double y )
    {
        pushEvent( window, ttb::events::MouseMove( x, y ) );
    }

    void callbackWindowSize( GLFWwindow* window, int width, int height )
    {
        auto wnd = reinterpret_cast< ttb::linux::Window* >( glfwGetWindowUserPointer( window ) );

        wnd->mode( ttb::Window::Mode( static_cast< uint16_t >( width ),
                                      static_cast< uint16_t >( height ),
                                      wnd->mode().flags() ) );

        pushEvent( window, ttb::events::WindowResize( *wnd ) );
    }

    void callbackScroll( GLFWwindow* window, double xoffset, double yoffset )
    {
        pushEvent( window, ttb::events::Scroll( xoffset, yoffset ) );
    }

    std::string convertErrorType( GLenum type )
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

    std::string convertErrorSeverity( GLenum severity )
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
