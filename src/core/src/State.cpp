#include <ttb/core/State.hpp>

#include <ttb/core/RenderTarget.hpp>
#include <ttb/core/shader/Program.hpp>
#include <ttb/core/window/Window.hpp>

#include <iostream>

namespace ttb
{
    State::State()
    {
    }

    State::~State()
    {
    }

    void State::pushTarget( std::shared_ptr< RenderTarget > const& target )
    {
        m_renderTargetStack.push( target );
        target->begin( *this );
    }

    void State::popTarget()
    {
        m_renderTargetStack.top()->end( *this );
        m_renderTargetStack.pop();

        if( !m_renderTargetStack.empty() )
        {
            m_renderTargetStack.top()->begin( *this );
        }
    }

    RenderTarget const& State::renderTarget() const
    {
        if( m_renderTargetStack.empty() )
        {
            throw std::runtime_error( "Requested top of empty target stack" );
        }

        return *m_renderTargetStack.top();
    }

    void State::pushProgram( std::shared_ptr< Program const > const& program )
    {
        if( m_programStack.empty() )
        {
            glGetIntegerv( GL_CURRENT_PROGRAM, &m_parentProgram );
        }

        program->use();
        m_programStack.push( program );
    }

    void State::popProgram()
    {
        m_programStack.pop();

        if( m_programStack.empty() )
        {
            glUseProgram( m_parentProgram );
        }
        else
        {
            m_programStack.top()->use();
        }
    }

    Program const& State::program()
    {
        return *m_programStack.top();
    }

    void State::pushArrayObject( GLuint arrayObject )
    {
        if( m_arrayObjectStack.empty() )
        {
            glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &m_parentArrayObject );
        }

        glBindVertexArray( arrayObject );
        m_arrayObjectStack.push( arrayObject );
    }

    void State::popArrayObject()
    {
        m_arrayObjectStack.pop();

        if( m_arrayObjectStack.empty() )
        {
            glBindVertexArray( m_parentArrayObject );
        }
        else
        {
            glBindVertexArray( m_arrayObjectStack.top() );
        }
    }

    void State::pushFramebuffer( GLuint framebufferObject )
    {
        if( m_framebufferObjectStack.empty() )
        {
            glGetIntegerv( GL_FRAMEBUFFER_BINDING, &m_parentFramebufferObject );
        }

        glBindFramebuffer( GL_FRAMEBUFFER, framebufferObject );
        m_framebufferObjectStack.push( framebufferObject );
    }

    void State::popFramebuffer()
    {
        m_framebufferObjectStack.pop();

        if( m_framebufferObjectStack.empty() )
        {
            glBindFramebuffer( GL_FRAMEBUFFER, m_parentFramebufferObject );
        }
        else
        {
            glBindFramebuffer( GL_FRAMEBUFFER, m_framebufferObjectStack.top() );
        }
    }

    void State::pushViewport( Viewport const& viewport )
    {
        m_viewportStack.push( viewport );

        glViewport( viewport.getX(), viewport.getY(), viewport.getWidth(), viewport.getHeight() );
    }

    void State::popViewport()
    {
        m_viewportStack.pop();
        if( !m_viewportStack.empty() )
        {
            auto const& viewport = m_viewportStack.top();
            glViewport(
                viewport.getX(), viewport.getY(), viewport.getWidth(), viewport.getHeight() );
        }
    }

    void State::apply()
    {
        if( !m_programStack.empty() )
        {
            auto& program = *m_programStack.top();

            for( auto const& uniformPair : m_uniforms )
            {
                program.applyUniform( uniformPair.first, *uniformPair.second );
            }
        }
    }
}
