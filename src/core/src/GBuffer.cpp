#include <ttb/core/GBuffer.hpp>

#include <ttb/core/gl.hpp>

#include <ttb/core/State.hpp>
#include <ttb/core/texture/Texture2D.hpp>

#include <cassert>


namespace
{
    void checkFramebufferStatus();
}

#ifdef PLATFORM_ANDROID
void glFramebufferTexture( GLenum, GLenum, GLuint, GLint )
{
}
#endif


namespace ttb
{
    // GBuffer
    //=================================================================================================

    GBuffer::Modifier GBuffer::create()
    {
        return modify( std::shared_ptr< GBuffer >( new GBuffer() ) );
    }

    GBuffer::~GBuffer()
    {
        glDeleteFramebuffers( 1, &m_frameBufferObject );
    }

    std::shared_ptr< Texture2D > const& GBuffer::drawBuffer( uint8_t unit ) const
    {
        return m_drawBuffers[ unit ];
    }

    std::shared_ptr< Texture2D > const& GBuffer::depthBuffer() const
    {
        return m_depthBuffer;
    }

    auto GBuffer::viewport() const -> Viewport
    {
        return { { 0, 0 }, { m_drawBuffers[ 0 ]->width(), m_drawBuffers[ 0 ]->height() } };
    }

    void GBuffer::begin( State::Data& data ) const
    {
        if( data.frameBufferObjectStack.empty() )
        {
            GLint parentFrameBufferObject;
            glGetIntegerv( GL_FRAMEBUFFER_BINDING, &parentFrameBufferObject );

            data.frameBufferObjectStack.push( parentFrameBufferObject );
        }

        data.frameBufferObjectStack.push( m_frameBufferObject );
    }

    void GBuffer::suspend( State::Data& /* data */ ) const
    {
        // Do nothing
    }

    void GBuffer::resume( State::Data& /* data */ ) const
    {
        glBindFramebuffer( GL_FRAMEBUFFER, m_frameBufferObject );
        glDrawBuffers( m_drawBufferIDs.size(), &m_drawBufferIDs[ 0 ] );
    }

    void ttb::GBuffer::end( State::Data& data ) const
    {
        data.frameBufferObjectStack.pop();

        glBindFramebuffer( GL_FRAMEBUFFER, data.frameBufferObjectStack.top() );
    }

    GBuffer::GBuffer()
    {
        glGenFramebuffers( 1, &m_frameBufferObject );
    }


    // GBufferModifier
    //=================================================================================================

    GBuffer::Modifier& GBuffer::Modifier::drawBuffer( uint8_t unit,
                                                      std::shared_ptr< Texture2D > buffer )
    {
        if( m_buffer->m_drawBuffers.size() <= unit )
        {
            m_buffer->m_drawBuffers.resize( unit + 1 );
            m_buffer->m_drawBufferIDs.resize( unit + 1 );
        }

        if( !buffer )
        {
            glFramebufferTexture( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + unit, 0, 0 );
        }
        else
        {
            assert( buffer->width() != 0 );
            assert( buffer->height() != 0 );

            if( buffer->target() == GL_TEXTURE_2D_MULTISAMPLE )
            {
                glFramebufferTexture(
                    GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + unit, buffer->object(), 0 );
            }
            else
            {
                glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER,
                                        GL_COLOR_ATTACHMENT0 + unit,
                                        buffer->target(),
                                        buffer->object(),
                                        0 );
            }
        }

        checkFramebufferStatus();

        m_buffer->m_drawBuffers[ unit ] = std::move( buffer );
        m_buffer->m_drawBufferIDs[ unit ] = GL_COLOR_ATTACHMENT0 + unit;

        return *this;
    }

    GBuffer::Modifier& GBuffer::Modifier::depthBuffer( std::shared_ptr< Texture2D > buffer )
    {
        if( buffer )
        {
            assert( buffer->width() != 0 );
            assert( buffer->height() != 0 );

            glFramebufferTexture2D(
                GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, buffer->target(), buffer->object(), 0 );
        }
        else
        {
            glFramebufferTexture( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0 );
        }

        checkFramebufferStatus();

        m_buffer->m_depthBuffer = std::move( buffer );

        return *this;
    }

    std::shared_ptr< GBuffer > GBuffer::Modifier::finish()
    {
        glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
        return std::exchange( m_buffer, std::shared_ptr< GBuffer >() );
    }

    GBuffer::Modifier::Modifier( std::shared_ptr< GBuffer > buffer )
        : m_buffer( std::move( buffer ) )
    {
        glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_buffer->m_frameBufferObject );
    }


    GBuffer::Modifier modify( std::shared_ptr< GBuffer > buffer )
    {
        return { std::move( buffer ) };
    }
}


namespace
{
    void checkFramebufferStatus()
    {
        auto const status = glCheckFramebufferStatus( GL_DRAW_FRAMEBUFFER );

        switch( status )
        {
            case GL_FRAMEBUFFER_COMPLETE:
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                return;

            default:
                throw std::runtime_error( "Framebuffer status error: " + std::to_string( status ) );
        }
    }
}
