#include <ttb/core/window/WindowCreator.hpp>

#include "WindowImpl.hpp"

#include <ttb/core/gl.hpp>

#include <iostream>


namespace ttb
{
    namespace
    {
        int glfwBool( bool value );

        GLFWwindow* createWindow( bool firstStartup,
                                  std::string const& title,
                                  WindowMode const& mode,
                                  GLFWwindow* sharedWindow );

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

    std::unique_ptr< Window > WindowCreator::create( std::string const& title,
                                                     WindowMode const& mode )
    {
        bool const firstStartup = ( Window::Impl::s_windowCount == 0 );
        auto const handle = createWindow( firstStartup, title, mode, nullptr );

        return std::unique_ptr< Window >{ new Window{
            std::unique_ptr< Window::Impl >{ new Window::Impl{ handle, title, mode } } } };
    }

    std::unique_ptr< Window > WindowCreator::create( std::string const& title,
                                                     WindowMode const& mode,
                                                     Window const& sharedWindow )
    {
        bool const firstStartup = ( Window::Impl::s_windowCount == 0 );
        auto const handle =
            createWindow( firstStartup, title, mode, sharedWindow.m_impl->m_handle );

        return std::unique_ptr< Window >{ new Window{
            std::unique_ptr< Window::Impl >{ new Window::Impl{ handle, title, mode } } } };
    }
}


namespace ttb
{
    namespace
    {
        inline int glfwBool( bool value )
        {
            return value ? GLFW_TRUE : GLFW_FALSE;
        }

        GLFWwindow* createWindow( bool firstStart,
                                  std::string const& title,
                                  WindowMode const& mode,
                                  GLFWwindow* sharedWindow )
        {
            if( firstStart )
            {
                glfwInit();
            }

            glfwSetErrorCallback( callbackErrorGLFW );

            glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
            glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
            // glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
            glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#ifndef NDEBUG
            glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );
#endif

            GLFWmonitor* const monitor =
                mode.flag( WindowFlag::FULLSCREEN ) ? glfwGetPrimaryMonitor() : nullptr;

            glfwWindowHint( GLFW_FLOATING, glfwBool( mode.flag( WindowFlag::FLOATING ) ) );
            glfwWindowHint( GLFW_RESIZABLE, glfwBool( mode.flag( WindowFlag::RESIZABLE ) ) );
            glfwWindowHint( GLFW_VISIBLE, glfwBool( !mode.flag( WindowFlag::HIDDEN ) ) );

            auto const handle = glfwCreateWindow(
                mode.width(), mode.height(), title.c_str(), monitor, sharedWindow );

            // Initialize OpenGL context
            {
                glfwMakeContextCurrent( handle );

                glfwSwapInterval( 0 );

                if( firstStart )
                {
                    // workaround for glew <= 1.13
                    // see https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library
                    glewExperimental = GL_TRUE;

                    if( auto const error = glewInit(); error != GLEW_OK )
                    {
                        throw std::runtime_error( "GLEW error: " +
                                                  std::string( reinterpret_cast< char const* >(
                                                      glewGetErrorString( error ) ) ) );
                    }
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

            return handle;
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
