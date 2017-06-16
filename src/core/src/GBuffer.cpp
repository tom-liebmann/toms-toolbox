#include <ttb/core/GBuffer.hpp>

#include <ttb/core/State.hpp>
#include <ttb/core/texture/Texture2D.hpp>

using namespace ttb;

ttb::GBuffer::GBuffer()
{
    glGenFramebuffers( 1, &m_frameBufferObject );
}

ttb::GBuffer::~GBuffer()
{
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
    glDeleteFramebuffers( 1, &m_frameBufferObject );
}

uint16_t ttb::GBuffer::getWidth() const
{
    return m_drawBuffers[ 0 ]->width();
}

uint16_t ttb::GBuffer::getHeight() const
{
    return m_drawBuffers[ 0 ]->height();
}

const std::shared_ptr< Texture2D >& ttb::GBuffer::getDrawBuffer( int8_t unit ) const
{
    return m_drawBuffers[ unit ];
}

const std::shared_ptr< Texture2D >& ttb::GBuffer::getDepthBuffer() const
{
    return m_depthBuffer;
}

void ttb::GBuffer::setDrawBuffer( uint8_t unit, std::shared_ptr< Texture2D > buffer )
{
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_frameBufferObject );

    if( m_drawBuffers.size() <= unit )
    {
        m_drawBuffers.resize( unit + 1 );
        m_drawBufferIDs.resize( unit + 1 );
    }

    glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + unit, GL_TEXTURE_2D,
                            buffer->object(), 0 );

    m_drawBuffers[ unit ] = std::move( buffer );
    m_drawBufferIDs[ unit ] = GL_COLOR_ATTACHMENT0 + unit;

    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
}

void ttb::GBuffer::setDepthBuffer( std::shared_ptr< Texture2D > buffer )
{
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_frameBufferObject );

    glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                            buffer ? buffer->object() : 0, 0 );

    m_depthBuffer = std::move( buffer );

    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
}

size_t ttb::GBuffer::width() const
{
    return m_drawBuffers[ 0 ]->width();
}

size_t ttb::GBuffer::height() const
{
    return m_drawBuffers[ 0 ]->height();
}

void ttb::GBuffer::begin( State& state ) const
{
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_frameBufferObject );

    if( m_drawBufferIDs.size() == 1 )
        glDrawBuffer( m_drawBufferIDs[ 0 ] );
    else
        glDrawBuffers( m_drawBufferIDs.size(), &m_drawBufferIDs[ 0 ] );

    state.pushViewport( Viewport( 0, 0, getWidth(), getHeight() ) );
}

void ttb::GBuffer::end( State& state ) const
{
    state.popViewport();

    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
}
