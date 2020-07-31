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
    class Window::Impl
    {
    public:
        Impl( std::string title, Mode const& mode, GLFWwindow* sharedWindow );

        ~Impl();

        void window( Window& wnd );

        Window& window();

        EventOutput& eventOutput();

        void mode( Mode const& mode );

        Mode const& mode() const;

        std::string const& title() const;

        GLFWwindow* handle() const;

        void update();

        void resize( uint16_t width, uint16_t height );

    private:
        static size_t& windowCount();

        Window* m_window;

        EventOutput m_eventOutput;

        Mode m_mode;

        std::string m_title;

        GLFWwindow* m_handle;
    };


    std::shared_ptr< Window > Window::create( std::string const& title, Window::Mode const& mode )
    {
        return std::shared_ptr< Window >(
            new Window( std::make_unique< Window::Impl >( title, mode, nullptr ) ) );
    }

    std::shared_ptr< Window > Window::create( std::string const& title,
                                              Window::Mode const& mode,
                                              ttb::Window const& sharedWindow )
    {
        return std::shared_ptr< Window >( new Window(
            std::make_unique< Window::Impl >( title, mode, sharedWindow.impl().handle() ) ) );
    }

    void Window::resize( uint16_t width, uint16_t height )
    {
        m_impl->resize( width, height );
    }

    void Window::update()
    {
        m_impl->update();
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
        glfwMakeContextCurrent( m_impl->handle() );
        return true;
    }

    bool Window::unuse()
    {
        glfwMakeContextCurrent( nullptr );
        return true;
    }

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

    Window::Window( std::unique_ptr< Impl > impl ) : m_impl{ std::move( impl ) }
    {
        m_impl->window( *this );
    }


    Window::Impl::Impl( std::string title, Mode const& mode, GLFWwindow* sharedWindow )
        : m_mode{ mode }, m_title{ std::move( title ) }
    {
        if( windowCount() == 0 )
        {
            glfwInit();
        }

        glfwSetErrorCallback( callbackErrorGLFW );

        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
        // glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#ifndef NDEBUG
        std::cout << "glfw version: " << glfwGetVersionString() << std::endl;
        std::cout << "glew version: " << glewGetString( GLEW_VERSION ) << std::endl;

        glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );
#endif

        GLFWmonitor* monitor =
            mode.flag( Window::Flag::FULLSCREEN ) ? glfwGetPrimaryMonitor() : nullptr;

        glfwWindowHint( GLFW_FLOATING, mode.flag( Window::Flag::FLOATING ) ? GL_TRUE : GL_FALSE );
        glfwWindowHint( GLFW_RESIZABLE, mode.flag( Window::Flag::RESIZABLE ) ? GL_TRUE : GL_FALSE );

        if( mode.flag( Window::Flag::HIDDEN ) )
        {
            glfwWindowHint( GLFW_VISIBLE, GL_FALSE );
            m_handle = glfwCreateWindow( mode.width(), mode.height(), "", nullptr, sharedWindow );
        }
        else
        {
            m_handle = glfwCreateWindow(
                mode.width(), mode.height(), title.c_str(), monitor, sharedWindow );
        }

        glfwMakeContextCurrent( m_handle );

        glfwSwapInterval( 0 );

        if( windowCount() == 0 )
        {
            // workaround for glew <= 1.13
            // see https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library
            glewExperimental = GL_TRUE;

            if( auto error = glewInit(); error != GLEW_OK )
            {
                throw std::runtime_error(
                    "GLEW error: " +
                    std::string( reinterpret_cast< char const* >( glewGetErrorString( error ) ) ) );
            }
        }

#ifndef NDEBUG

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

        ++windowCount();

        glfwMakeContextCurrent( nullptr );
    }

    Window::Impl::~Impl()
    {
        glfwDestroyWindow( m_handle );

        --windowCount();

        if( windowCount() == 0 )
        {
            glfwTerminate();
        }
    }

    void Window::Impl::window( Window& wnd )
    {
        m_window = &wnd;
    }

    Window& Window::Impl::window()
    {
        return *m_window;
    }

    Window::EventOutput& Window::Impl::eventOutput()
    {
        return m_eventOutput;
    }

    void Window::Impl::mode( Mode const& mode )
    {
        m_mode = mode;
    }

    Window::Mode const& Window::Impl::mode() const
    {
        return m_mode;
    }

    std::string const& Window::Impl::title() const
    {
        return m_title;
    }

    GLFWwindow* Window::Impl::handle() const
    {
        return m_handle;
    }

    void Window::Impl::update()
    {
        glfwSwapBuffers( m_handle );
        glfwPollEvents();
    }

    void Window::Impl::resize( uint16_t width, uint16_t height )
    {
        glfwSetWindowSize( m_handle, width, height );

        m_mode = Mode( width, height, mode().flags() );

        m_eventOutput.call( ttb::events::WindowResize( *m_window ) );
    }

    size_t& Window::Impl::windowCount()
    {
        static size_t s_windowCount = 0;
        return s_windowCount;
    }
}


namespace
{
    void pushEvent( GLFWwindow* window, ttb::Event const& event )
    {
        auto wnd = reinterpret_cast< ttb::Window::Impl* >( glfwGetWindowUserPointer( window ) );

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
        auto wnd = reinterpret_cast< ttb::Window::Impl* >( glfwGetWindowUserPointer( window ) );

        pushEvent( window, ttb::events::WindowClose( wnd->window() ) );
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
        auto wnd = reinterpret_cast< ttb::Window::Impl* >( glfwGetWindowUserPointer( window ) );

        wnd->mode( ttb::Window::Mode( static_cast< uint16_t >( width ),
                                      static_cast< uint16_t >( height ),
                                      wnd->mode().flags() ) );

        pushEvent( window, ttb::events::WindowResize( wnd->window() ) );
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
