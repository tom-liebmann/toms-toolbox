#include <ttb/core/GBuffer.hpp>

#include <ttb/core/State.hpp>
#include <ttb/core/texture/Texture2D.hpp>


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

    size_t GBuffer::width() const
    {
        return m_drawBuffers[ 0 ]->width();
    }

    size_t ttb::GBuffer::height() const
    {
        return m_drawBuffers[ 0 ]->height();
    }

    void GBuffer::begin( State& state ) const
    {
        state.pushFramebuffer( m_frameBufferObject );

        if( m_drawBufferIDs.size() == 1 )
        {
            glDrawBuffer( m_drawBufferIDs[ 0 ] );
        }
        else
        {
            glDrawBuffers( m_drawBufferIDs.size(), &m_drawBufferIDs[ 0 ] );
        }

        state.pushViewport( Viewport( 0, 0, width(), height() ) );
    }

    void ttb::GBuffer::end( State& state ) const
    {
        state.popViewport();
        state.popFramebuffer();
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

        glFramebufferTexture2D(
            GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + unit, GL_TEXTURE_2D, buffer->object(), 0 );

        m_buffer->m_drawBuffers[ unit ] = std::move( buffer );
        m_buffer->m_drawBufferIDs[ unit ] = GL_COLOR_ATTACHMENT0 + unit;

        return *this;
    }

    GBuffer::Modifier& GBuffer::Modifier::depthBuffer( std::shared_ptr< Texture2D > buffer )
    {
        glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER,
                                GL_DEPTH_ATTACHMENT,
                                GL_TEXTURE_2D,
                                buffer ? buffer->object() : 0,
                                0 );

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
