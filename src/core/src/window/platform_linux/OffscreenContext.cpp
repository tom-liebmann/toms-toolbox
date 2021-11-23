#include <ttb/core/window/OffscreenContext.hpp>

#include "WindowImpl.hpp"

#include <ttb/core/gl.hpp>

#include <GLFW/glfw3.h>


namespace ttb
{
    class OffscreenContext::Impl
    {
    public:
        explicit Impl( Context& sharedContext );

        Impl();

        ~Impl();

        bool use();

        bool unuse();

    private:
        Impl( GLFWwindow* sharedWindow );

        GLFWwindow* m_handle;
    };


    OffscreenContext::OffscreenContext( Context& sharedContext )
        : m_impl{ new Impl{ sharedContext } }
    {
    }

    OffscreenContext::OffscreenContext() : m_impl{ new Impl{} }
    {
    }

    OffscreenContext::~OffscreenContext() = default;

    bool OffscreenContext::use()
    {
        return m_impl->use();
    }

    bool OffscreenContext::unuse()
    {
        return m_impl->unuse();
    }


    OffscreenContext::Impl::Impl( Context& sharedContext )
        : Impl{ dynamic_cast< WindowImpl& >( sharedContext ).handle() }
    {
    }

    OffscreenContext::Impl::Impl() : Impl{ nullptr }
    {
    }

    OffscreenContext::Impl::Impl( GLFWwindow* sharedWindow )
    {
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
        // glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

        glfwWindowHint( GLFW_VISIBLE, GLFW_FALSE );

        m_handle = glfwCreateWindow( 1, 1, "", nullptr, sharedWindow );
    }

    OffscreenContext::Impl::~Impl()
    {
    }

    bool OffscreenContext::Impl::use()
    {
        glfwMakeContextCurrent( m_handle );
        return true;
    }

    bool OffscreenContext::Impl::unuse()
    {
        glfwMakeContextCurrent( nullptr );
        return true;
    }
}
