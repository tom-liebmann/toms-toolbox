#include <ttb/core/State.hpp>

#include <ttb/core/RenderTarget.hpp>
#include <ttb/core/Window.hpp>
#include <ttb/core/shader/Program.hpp>

#include <iostream>

namespace ttb
{
    State::State() : m_projectionMatrixSet( false ), m_modelViewMatrixSet( false )
    {
        m_modelViewMatrixStack.emplace( MatrixFactory< float >::identity() );
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

    void State::projectionMatrix( Matrix< float, 4, 4 > const& matrix )
    {
        m_projectionMatrix = matrix;
        m_projectionMatrixSet = false;
    }

    Matrix< float, 4, 4 > const& State::projectionMatrix() const
    {
        return m_projectionMatrix;
    }

    void State::pushMatrix()
    {
        m_modelViewMatrixStack.push( m_modelViewMatrixStack.top() );
    }

    void State::popMatrix()
    {
        m_modelViewMatrixStack.pop();
        m_modelViewMatrixSet = false;
    }

    void State::setMatrix( Matrix< float, 4, 4 > const& matrix )
    {
        m_modelViewMatrixStack.top() = matrix;
        m_modelViewMatrixSet = false;
    }

    void State::applyMatrix( Matrix< float, 4, 4 > const& matrix )
    {
        m_modelViewMatrixStack.top() *= matrix;
        m_modelViewMatrixSet = false;
    }

    Matrix< float, 4, 4 > const& State::modelViewMatrix() const
    {
        return m_modelViewMatrixStack.top();
    }

    void State::pushProgram( std::shared_ptr< Program > const& program )
    {
        program->use();
        m_programStack.push( program );

        m_projectionMatrixSet = false;
        m_modelViewMatrixSet = false;
    }

    void State::popProgram()
    {
        m_programStack.top()->unuse();
        m_programStack.pop();

        if( !m_programStack.empty() )
        {
            m_programStack.top()->use();
        }

        m_projectionMatrixSet = false;
        m_modelViewMatrixSet = false;
    }

    Program& State::program()
    {
        return *m_programStack.top();
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
            auto& program = m_programStack.top();

            if( !m_projectionMatrixSet )
            {
                GLint location = program->getUniformLocation( "u_projectionMatrix" );
                if( location != -1 )
                {
                    program->setUniform( location, m_projectionMatrix );
                }

                location = program->getUniformLocation( "u_invProjectionMatrix" );
                if( location != -1 )
                {
                    program->setUniform( location, invert( m_projectionMatrix ) );
                }

                m_projectionMatrixSet = true;
            }

            if( !m_modelViewMatrixSet )
            {
                GLint location = program->getUniformLocation( "u_modelViewMatrix" );
                if( location != -1 )
                {
                    program->setUniform( location, m_modelViewMatrixStack.top() );
                }

                location = program->getUniformLocation( "u_invModelViewMatrix" );
                if( location != -1 )
                {
                    program->setUniform( location, invert( m_modelViewMatrixStack.top() ) );
                }

                m_modelViewMatrixSet = true;
            }
        }
    }
}
