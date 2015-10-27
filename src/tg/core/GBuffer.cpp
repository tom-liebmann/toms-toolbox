#include "GBuffer.hpp"

#include <tg/core/Texture2D.hpp>
#include <tg/core/State.hpp>

using namespace tg;

tg::GBuffer::GBuffer()
{
    glGenFramebuffers( 1, &m_frameBufferObject );
}

tg::GBuffer::~GBuffer()
{
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
    glDeleteFramebuffers( 1, &m_frameBufferObject );
}

uint16_t tg::GBuffer::getWidth() const
{
    return m_drawBuffers[ 0 ]->getWidth();
}

uint16_t tg::GBuffer::getHeight() const
{
    return m_drawBuffers[ 0 ]->getHeight();
}

const std::shared_ptr< Texture2D >& tg::GBuffer::getDrawBuffer( int8_t unit ) const
{
    return m_drawBuffers[ unit ];
}

const std::shared_ptr< Texture2D >& tg::GBuffer::getDepthBuffer() const
{
    return m_depthBuffer;
}

void tg::GBuffer::setDrawBuffer( uint8_t unit, std::shared_ptr< Texture2D > buffer )
{
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_frameBufferObject );

    if( m_drawBuffers.size() <= unit )
    {
        m_drawBuffers.resize( unit + 1 );
        m_drawBufferIDs.resize( unit + 1 );
    }

    glFramebufferTexture2D(
        GL_DRAW_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0 + unit,
        GL_TEXTURE_2D,
        buffer->getTextureObject(),
        0 ); 

    m_drawBuffers[ unit ] = std::move( buffer );
    m_drawBufferIDs[ unit ] = GL_COLOR_ATTACHMENT0 + unit;
}

void tg::GBuffer::setDepthBuffer( std::shared_ptr< Texture2D > buffer )
{
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_frameBufferObject );

    glFramebufferTexture2D(
        GL_DRAW_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        buffer ? buffer->getTextureObject() : 0,
        0); 

    m_depthBuffer = std::move( buffer );
}

void tg::GBuffer::begin( State& state ) const
{
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_frameBufferObject );

    if( m_drawBufferIDs.size() == 1 )
        glDrawBuffer( m_drawBufferIDs[ 0 ] );
    else
        glDrawBuffers( m_drawBufferIDs.size(), &m_drawBufferIDs[ 0 ] );

    state.pushViewport( Viewport( 0, 0, getWidth(), getHeight() ) );
}

void tg::GBuffer::end( State& state ) const
{
    state.popViewport();

    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
}
