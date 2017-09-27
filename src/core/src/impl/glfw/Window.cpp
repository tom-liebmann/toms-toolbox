#include <ttb/core/window/Window.hpp>

#include <GLFW/glfw3.h>


#ifdef WIN32
#define STDCALL __attribute__( ( __stdcall__ ) )
#else
#define STDCALL
#endif

namespace
{
    void pushEvent( GLFWwindow* window, std::unique_ptr< ttb::Event > event );

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
    class Window::Impl
    {
    public:
        Impl( std::string const& title, WindowMode const& mode );

        ~Impl();

        void update();

    private:
        static size_t s_windowCount;

        GLFWwindow* m_handle;
    };
}


namespace ttb
{
    Window::Window( std::string const& title, WindowMode const& mode )
        : m_title( title ), m_mode( mode ), m_impl( std::make_unique< Impl >( title, mode ) )
    {
    }

    void Window::mode( WindowMode const& mode )
    {
    }

    WindowMode const& Window::mode() const
    {
        return m_mode;
    }

    std::string const& Window::title() const
    {
        return m_title;
    }

    std::shared_ptr< Provider< SlotType::ACTIVE, Event const& > > const& Window::eventOutput() const
    {
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
            glfwInit();
        }

        glfwSetErrorCallback( callbackErrorGLFW );

//        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
// glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
// glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#ifndef NDEBUG
        glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, 1 );
#endif

        m_handle = glfwCreateWindow(
            mode.width(), mode.height(), title.c_str(), mode.getMonitor().getHandle(), nullptr );

        glfwMakeContextCurrent( m_handle );

#ifdef GLEW_STATIC
        if( s_windowCount == 0 )
        {
            glewInit();
        }
#endif

#ifndef NDEBUG
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
    }

    Window::Impl::~Impl()
    {
        glfwDestroyWindow( m_handle );

        --s_windowCount;

        if( s_windowCount == 0 )
        {
            glfwTerminate();
        }
    }

    void Window::Impl::update()
    {
        glfwSwapBuffers( m_handle );
        glfwPollEvents();
    }
}


namespace
{
    void pushEvent( GLFWwindow* window, std::unique_ptr< ttb::Event > event )
    {
        auto wnd = reinterpret_cast< ttb::Window* >( glfwGetWindowUserPointer( window ) );

        auto eventManager = wnd->eventManager();

        if( eventManager )
            eventManager->pushEvent( std::move( event ) );
    }

    void callbackErrorGLFW( int error, char const* description )
    {
        std::cout << "GLFW error (" << error << "): " << description << std::endl;
    }

    STDCALL void callbackErrorOpenGL( GLenum source,
                                      GLenum type,
                                      GLuint id,
                                      GLenum severity,
                                      GLsizei length,
                                      GLchar const* message,
                                      void const* userParam )
    {
        std::cout << "OpenGL error:" << std::endl;
        std::cout << "    Type: " << convertErrorType( type ) << std::endl;
        std::cout << "    Severity: " << convertErrorSeverity( severity ) << std::endl;
        std::cout << "    Message: " << message << std::endl;
    }

    void callbackWindowClose( GLFWwindow* window )
    {
        auto wnd = reinterpret_cast< ttb::Window::Impl* >( glfwGetWindowUserPointer( window ) );

        pushEvent( window, std::unique_ptr< ttb::Event >( new ttb::events::WindowClose( *wnd ) ) );
    }

    void callbackKey( GLFWwindow* window, int key, int scancode, int action, int mods )
    {
        switch( action )
        {
            case GLFW_PRESS:
                pushEvent( window,
                           std::unique_ptr< ttb::Event >(
                               new ttb::events::Key( key, ttb::events::Key::Action::DOWN ) ) );
                break;

            case GLFW_RELEASE:
                pushEvent( window,
                           std::unique_ptr< ttb::Event >(
                               new ttb::events::Key( key, ttb::events::Key::Action::UP ) ) );
                break;
        }
    }

    void callbackMouseButton( GLFWwindow* window, int button, int action, int mods )
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
                   std::unique_ptr< ttb::Event >( new ttb::events::MouseButton(
                       mouseButton,
                       action == GLFW_PRESS ? ttb::events::MouseButton::Action::DOWN
                                            : ttb::events::MouseButton::Action::UP,
                       mouseX,
                       mouseY ) ) );
    }

    void callbackMouseMove( GLFWwindow* window, double x, double y )
    {
        pushEvent( window, std::unique_ptr< ttb::Event >( new ttb::events::MouseMove( x, y ) ) );
    }

    void callbackWindowSize( GLFWwindow* window, int width, int height )
    {
        auto wnd = reinterpret_cast< ttb::Window* >( glfwGetWindowUserPointer( window ) );

        wnd->mode( ttb::WindowMode( width, height, wnd->mode().isFullscreen() ) );

        pushEvent( window, std::unique_ptr< ttb::Event >( new ttb::events::WindowResize( *wnd ) ) );
    }

    void callbackScroll( GLFWwindow* window, double xoffset, double yoffset )
    {
        pushEvent( window,
                   std::unique_ptr< ttb::Event >( new ttb::events::Scroll( xoffset, yoffset ) ) );
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
