#include "State.hpp"

#include <tgMath/Matrix4f.hpp>
#include <tgCore/ShaderProgram.hpp>

#include <iostream>

using tgCore::State;

State::State()
    : m_projectionMatrixSet( false )
    , m_modelViewMatrixSet( false )
{
    m_projectionMatrix = new tgMath::Matrix4f();
    m_modelViewMatrixStack.push_back( new tgMath::Matrix4f() );
}

State::~State()
{
    delete m_projectionMatrix;

    for( auto& matrix : m_modelViewMatrixStack )
        delete matrix;
}

void State::setProjectionMatrix( const tgMath::Matrix4f& matrix )
{
    m_projectionMatrix->operator=( matrix );

    m_projectionMatrixSet = false;
}

void State::pushMatrix()
{
    m_modelViewMatrixStack.push_back( new tgMath::Matrix4f( *m_modelViewMatrixStack.back() ) );
}

void State::popMatrix()
{
    delete m_modelViewMatrixStack.back();
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

void State::pushShader( std::shared_ptr< tgCore::ShaderProgram > shader )
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
