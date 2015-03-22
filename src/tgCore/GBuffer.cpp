#include "GBuffer.hpp"

#include <tgCore/Texture2D.hpp>

using namespace tg;

GBuffer::GBuffer()
{
    glGenFramebuffers( 1, &m_frameBufferObject );
}

GBuffer::~GBuffer()
{
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
    glDeleteFramebuffers( 1, &m_frameBufferObject );
}

uint16_t GBuffer::getWidth() const
{
    return m_drawBuffers[ 0 ]->getWidth();
}

uint16_t GBuffer::getHeight() const
{
    return m_drawBuffers[ 0 ]->getHeight();
}

const std::shared_ptr< Texture2D >& GBuffer::getDrawBuffer( int8_t unit ) const
{
    return m_drawBuffers[ unit ];
}

const std::shared_ptr< Texture2D >& GBuffer::getDepthBuffer() const
{
    return m_depthBuffer;
}

void GBuffer::setDrawBuffer( uint8_t unit, std::shared_ptr< Texture2D > buffer )
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

    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

    m_drawBuffers[ unit ] = std::move( buffer );
    m_drawBufferIDs[ unit ] = GL_COLOR_ATTACHMENT0 + unit;
}

void GBuffer::setDepthBuffer( std::shared_ptr< Texture2D > buffer )
{
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_frameBufferObject );

    glFramebufferTexture2D(
        GL_DRAW_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        buffer->getTextureObject(),
        0); 

    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

    m_depthBuffer = std::move( buffer );
}

void GBuffer::begin() const
{
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_frameBufferObject );

    if( m_drawBufferIDs.size() == 1 )
        glDrawBuffer( m_drawBufferIDs[ 0 ] );
    else
        glDrawBuffers( m_drawBufferIDs.size(), &m_drawBufferIDs[ 0 ] );

//    graphState().pushViewport( 0, 0, m_width, m_height );
}

void GBuffer::end() const
{
//    graphState().popViewport();

    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
}
