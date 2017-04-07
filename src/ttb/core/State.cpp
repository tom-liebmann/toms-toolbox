#include "State.hpp"

#include <ttb/core/ShaderProgram.hpp>
#include <ttb/core/Window.hpp>
#include <ttb/math/Matrix.hpp>

#include <iostream>

ttb::State::State( Window& window )
    : m_window( window )
    , m_projectionMatrixSet( false )
    , m_projectionMatrix( new ttb::Matrix4f() )
    , m_modelViewMatrixSet( false )
    , m_windowViewport(
          new Viewport( 0, 0, m_window.getMode().getWidth(), m_window.getMode().getHeight() ) )
{
    m_modelViewMatrixStack.emplace_back(
        new Matrix< float, 4, 4 >( MatrixFactory< float >::fromIdentity() ) );

    glViewport( m_windowViewport->getX(), m_windowViewport->getY(), m_windowViewport->getWidth(),
                m_windowViewport->getHeight() );
}

ttb::State::~State()
{
}

void ttb::State::setProjectionMatrix( const ttb::Matrix4f& matrix )
{
    m_projectionMatrix->operator=( matrix );

    m_projectionMatrixSet = false;
}

ttb::Matrix4f const& ttb::State::getProjectionMatrix() const
{
    return *m_projectionMatrix;
}

void ttb::State::pushMatrix()
{
    m_modelViewMatrixStack.push_back(
        std::unique_ptr< ttb::Matrix4f >( new ttb::Matrix4f( *m_modelViewMatrixStack.back() ) ) );
}

void ttb::State::popMatrix()
{
    m_modelViewMatrixStack.pop_back();

    m_modelViewMatrixSet = false;
}

void ttb::State::setMatrix( const ttb::Matrix4f& matrix )
{
    m_modelViewMatrixStack.back()->operator=( matrix );

    m_modelViewMatrixSet = false;
}

void ttb::State::applyMatrix( const ttb::Matrix4f& matrix )
{
    m_modelViewMatrixStack.back()->operator=( ( *m_modelViewMatrixStack.back() ) * matrix );

    m_modelViewMatrixSet = false;
}

ttb::Matrix4f const& ttb::State::getModelViewMatrix() const
{
    return *m_modelViewMatrixStack.back();
}

void ttb::State::pushShader( std::shared_ptr< ShaderProgram > shader )
{
    shader->use();
    m_shaderStack.push_back( std::move( shader ) );

    m_projectionMatrixSet = false;
    m_modelViewMatrixSet = false;
}

void ttb::State::popShader()
{
    m_shaderStack.back()->unuse();
    m_shaderStack.pop_back();

    if( !m_shaderStack.empty() )
        m_shaderStack.back()->use();

    m_projectionMatrixSet = false;
    m_modelViewMatrixSet = false;
}

void ttb::State::pushViewport( Viewport viewport )
{
    m_viewportStack.push_back( viewport );

    glViewport( viewport.getX(), viewport.getY(), viewport.getWidth(), viewport.getHeight() );
}

void ttb::State::popViewport()
{
    m_viewportStack.pop_back();
    if( !m_viewportStack.empty() )
    {
        const auto& viewport = m_viewportStack.back();
        glViewport( viewport.getX(), viewport.getY(), viewport.getWidth(), viewport.getHeight() );
    }
    else
    {
        const auto& viewport = *m_windowViewport;
        glViewport( viewport.getX(), viewport.getY(), viewport.getWidth(), viewport.getHeight() );
    }
}

void ttb::State::onWindowResize()
{
    *m_windowViewport =
        Viewport( 0, 0, m_window.getMode().getWidth(), m_window.getMode().getHeight() );

    if( m_viewportStack.empty() )
        glViewport( m_windowViewport->getX(), m_windowViewport->getY(),
                    m_windowViewport->getWidth(), m_windowViewport->getHeight() );
}

void ttb::State::apply()
{
    if( !m_shaderStack.empty() )
    {
        auto& shader = m_shaderStack.back();

        if( !m_projectionMatrixSet )
        {
            GLint location = shader->getUniformLocation( "u_projectionMatrix" );
            if( location != -1 )
                shader->setUniform( location, *m_projectionMatrix );

            location = shader->getUniformLocation( "u_invProjectionMatrix" );
            if( location != -1 )
                shader->setUniform( location, invert( *m_projectionMatrix ) );

            m_projectionMatrixSet = true;
        }

        if( !m_modelViewMatrixSet )
        {
            GLint location = shader->getUniformLocation( "u_modelViewMatrix" );
            if( location != -1 )
                shader->setUniform( location, *m_modelViewMatrixStack.back() );

            location = shader->getUniformLocation( "u_invModelViewMatrix" );
            if( location != -1 )
                shader->setUniform( location, invert( *m_modelViewMatrixStack.back() ) );

            m_modelViewMatrixSet = true;
        }
    }
}
