#include "State.hpp"

#include <ttb/core/RenderTarget.hpp>
#include <ttb/core/Window.hpp>
#include <ttb/core/shader/Program.hpp>

#include <iostream>

namespace ttb
{
    State::State() : m_projectionMatrixSet( false ), m_modelViewMatrixSet( false )
    {
        m_modelViewMatrixStack.emplace_back( MatrixFactory< float >::fromIdentity() );
    }

    State::~State()
    {
    }

    void State::pushTarget( std::shared_ptr< RenderTarget > const& target )
    {
        m_renderTargetStack.push_back( target );
        target->begin( *this );
    }

    void State::popTarget()
    {
        m_renderTargetStack.back()->end( *this );
        m_renderTargetStack.pop_back();
    }

    RenderTarget const& State::renderTarget() const
    {
        return *m_renderTargetStack.back();
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
        m_modelViewMatrixStack.push_back( m_modelViewMatrixStack.back() );
    }

    void State::popMatrix()
    {
        m_modelViewMatrixStack.pop_back();
        m_modelViewMatrixSet = false;
    }

    void State::setMatrix( Matrix< float, 4, 4 > const& matrix )
    {
        m_modelViewMatrixStack.back() = matrix;
        m_modelViewMatrixSet = false;
    }

    void State::applyMatrix( Matrix< float, 4, 4 > const& matrix )
    {
        m_modelViewMatrixStack.back() *= matrix;
        m_modelViewMatrixSet = false;
    }

    Matrix< float, 4, 4 > const& State::modelViewMatrix() const
    {
        return m_modelViewMatrixStack.back();
    }

    void State::pushProgram( std::shared_ptr< Program > const& program )
    {
        program->use();
        m_programStack.push_back( program );

        m_projectionMatrixSet = false;
        m_modelViewMatrixSet = false;
    }

    void State::popProgram()
    {
        m_programStack.back()->unuse();
        m_programStack.pop_back();

        if( !m_programStack.empty() )
            m_programStack.back()->use();

        m_projectionMatrixSet = false;
        m_modelViewMatrixSet = false;
    }

    Program& State::program()
    {
        return *m_programStack.back();
    }

    void State::pushViewport( Viewport const& viewport )
    {
        m_viewportStack.push_back( viewport );

        glViewport( viewport.getX(), viewport.getY(), viewport.getWidth(), viewport.getHeight() );
    }

    void State::popViewport()
    {
        m_viewportStack.pop_back();
        if( !m_viewportStack.empty() )
        {
            const auto& viewport = m_viewportStack.back();
            glViewport( viewport.getX(), viewport.getY(), viewport.getWidth(),
                        viewport.getHeight() );
        }
    }

    void State::apply()
    {
        if( !m_programStack.empty() )
        {
            auto& program = m_programStack.back();

            if( !m_projectionMatrixSet )
            {
                GLint location = program->getUniformLocation( "u_projectionMatrix" );
                if( location != -1 )
                    program->setUniform( location, m_projectionMatrix );

                location = program->getUniformLocation( "u_invProjectionMatrix" );
                if( location != -1 )
                    program->setUniform( location, invert( m_projectionMatrix ) );

                m_projectionMatrixSet = true;
            }

            if( !m_modelViewMatrixSet )
            {
                GLint location = program->getUniformLocation( "u_modelViewMatrix" );
                if( location != -1 )
                    program->setUniform( location, m_modelViewMatrixStack.back() );

                location = program->getUniformLocation( "u_invModelViewMatrix" );
                if( location != -1 )
                    program->setUniform( location, invert( m_modelViewMatrixStack.back() ) );

                m_modelViewMatrixSet = true;
            }
        }
    }
}
