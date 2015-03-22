#include "State.hpp"

#include <tgMath/Matrix4f.hpp>
#include <tgCore/ShaderProgram.hpp>
#include <tgCore/Window.hpp>

#include <iostream>

using namespace tg;

State::State( Window& window )
    : m_window( window )
    , m_projectionMatrixSet( false )
    , m_projectionMatrix( new tgMath::Matrix4f() )
    , m_modelViewMatrixSet( false )
    , m_windowViewport( new Viewport( 0, 0, m_window.getMode().getWidth(), m_window.getMode().getHeight() ) )
{
    m_modelViewMatrixStack.push_back( std::unique_ptr< tgMath::Matrix4f >( new tgMath::Matrix4f() ) );

    glViewport(
        m_windowViewport->getX(),
        m_windowViewport->getY(),
        m_windowViewport->getWidth(),
        m_windowViewport->getHeight()
    );
}

State::~State()
{ }

void State::setProjectionMatrix( const tgMath::Matrix4f& matrix )
{
    m_projectionMatrix->operator=( matrix );

    m_projectionMatrixSet = false;
}

void State::pushMatrix()
{
    m_modelViewMatrixStack.push_back( 
        std::unique_ptr< tgMath::Matrix4f >(
            new tgMath::Matrix4f( *m_modelViewMatrixStack.back() ) ) );
}

void State::popMatrix()
{
    m_modelViewMatrixStack.pop_back();

    m_modelViewMatrixSet = false;
}

void State::setMatrix( const tgMath::Matrix4f& matrix )
{
    m_modelViewMatrixStack.back()->operator=( matrix );

    m_modelViewMatrixSet = false;
}

void State::applyMatrix( const tgMath::Matrix4f& matrix )
{
    m_modelViewMatrixStack.back()->operator=(
        (*m_modelViewMatrixStack.back()) * matrix );

    m_modelViewMatrixSet = false;
}

void State::pushShader( std::shared_ptr< ShaderProgram > shader )
{
    shader->use();
    m_shaderStack.push_back( std::move( shader ) );

    m_projectionMatrixSet = false;
    m_modelViewMatrixSet = false;
}

void State::popShader()
{
    m_shaderStack.back()->unuse();
    m_shaderStack.pop_back();

    if( !m_shaderStack.empty() )
        m_shaderStack.back()->use();

    m_projectionMatrixSet = false;
    m_modelViewMatrixSet = false;
}

void State::pushViewport( Viewport viewport )
{
    m_viewportStack.push_back( viewport );

    glViewport(
        viewport.getX(),
        viewport.getY(),
        viewport.getWidth(),
        viewport.getHeight()
    );
}

void State::popViewport()
{
    m_viewportStack.pop_back();
    if( m_viewportStack.empty() )
    {
        const auto& viewport = m_viewportStack.back();
        glViewport(
            viewport.getX(),
            viewport.getY(),
            viewport.getWidth(),
            viewport.getHeight()
        );
    }
    else
    {
        const auto& viewport = *m_windowViewport;
        glViewport(
            viewport.getX(),
            viewport.getY(),
            viewport.getWidth(),
            viewport.getHeight()
        );
    }
}

void State::onWindowResize()
{
    *m_windowViewport = Viewport(
        0,
        0,
        m_window.getMode().getWidth(),
        m_window.getMode().getHeight()
    );

    if( m_viewportStack.empty() )
        glViewport(
            m_windowViewport->getX(),
            m_windowViewport->getY(),
            m_windowViewport->getWidth(),
            m_windowViewport->getHeight()
        );
}

void State::apply()
{
    if( !m_shaderStack.empty() )
    {
        auto& shader = m_shaderStack.back();

        if( !m_projectionMatrixSet )
        {
            GLint location = shader->getUniformLocation( "u_projectionMatrix" );
            if( location != -1 )
                shader->setUniform( location, *m_projectionMatrix );

            m_projectionMatrixSet = true;
        }

        if( !m_modelViewMatrixSet )
        {
            GLint location = shader->getUniformLocation( "u_modelViewMatrix" );
            if( location != -1 )
                shader->setUniform( location, *m_modelViewMatrixStack.back() );

            location = shader->getUniformLocation( "u_normalMatrix" );
            if( location != -1 )
                shader->setUniform( location, m_modelViewMatrixStack.back()->invert3x3() );

            m_modelViewMatrixSet = true;
        }
    }
}
