#include <ttb/core/texture/Texture2D.hpp>

#include <iostream>

namespace ttb
{
    Texture2D::Texture2D() : m_width( 0 ), m_height( 0 )
    {
    }

    Texture2D::Texture2D(
        uint16_t width, uint16_t height, GLint internalFormat, GLenum format, GLenum valueType )
        : m_width( width )
        , m_height( height )
        , m_internalFormat( internalFormat )
        , m_format( format )
        , m_valueType( valueType )
    {
        glBindTexture( GL_TEXTURE_2D, m_object );

        glTexImage2D(
            GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, m_valueType, NULL );

        glBindTexture( GL_TEXTURE_2D, 0 );
    }

    std::unique_ptr< Texture2DModifier > Texture2D::modify()
    {
        return std::unique_ptr< Texture2DModifier >( new Texture2DModifier( *this ) );
    }

    uint16_t Texture2D::width() const
    {
        return m_width;
    }

    uint16_t Texture2D::height() const
    {
        return m_height;
    }

    uint8_t Texture2D::bytesPerPixel() const
    {
        switch( m_internalFormat )
        {
            case GL_RGBA:
                return 4;

            default:
                throw std::runtime_error( "Unknown pixel format" );
        }
    }

    void Texture2D::bind( uint8_t slot )
    {
        glActiveTexture( GL_TEXTURE0 + slot );
        glBindTexture( GL_TEXTURE_2D, object() );
    }


    Texture2DModifier::Texture2DModifier( Texture2D& texture ) : m_texture( texture )
    {
        glBindTexture( GL_TEXTURE_2D, m_texture.object() );
    }

    Texture2DModifier::~Texture2DModifier()
    {
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

    void Texture2DModifier::upload(
        uint16_t x, uint16_t y, uint16_t width, uint16_t height, void const* data ) const
    {
        glTexSubImage2D( GL_TEXTURE_2D,
                         0,
                         x,
                         y,
                         width,
                         height,
                         m_texture.m_format,
                         m_texture.m_valueType,
                         data );
    }

    void Texture2DModifier::upload( uint16_t width,
                                    uint16_t height,
                                    GLint internalFormat,
                                    GLenum format,
                                    GLenum valueType,
                                    void const* data ) const
    {
        m_texture.m_width = width;
        m_texture.m_height = height;
        m_texture.m_internalFormat = internalFormat;
        m_texture.m_format = format;
        m_texture.m_valueType = valueType;

        glTexImage2D( GL_TEXTURE_2D,
                      0,
                      m_texture.m_internalFormat,
                      m_texture.m_width,
                      m_texture.m_height,
                      0,
                      m_texture.m_format,
                      m_texture.m_valueType,
                      data );
    }

    void Texture2DModifier::upload( void const* data ) const
    {
        glTexSubImage2D( GL_TEXTURE_2D,
                         0,
                         0,
                         0,
                         m_texture.m_width,
                         m_texture.m_height,
                         m_texture.m_format,
                         m_texture.m_valueType,
                         data );
    }

    void Texture2DModifier::download( size_t level, std::vector< uint8_t >& buffer ) const
    {
        GLint width, height;
        glGetTexLevelParameteriv( GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &width );
        glGetTexLevelParameteriv( GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &height );

        buffer.resize( width * height * m_texture.bytesPerPixel() );

        glGetTexImage(
            GL_TEXTURE_2D, level, m_texture.m_format, m_texture.m_valueType, buffer.data() );
    }

    void Texture2DModifier::minMagFilter( GLint minFilter, GLint magFilter ) const
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter );
    }

    void Texture2DModifier::depthMode( GLint mode ) const
    {
        glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, mode );
    }

    void Texture2DModifier::compareMode( GLint mode ) const
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, mode );
    }

    void Texture2DModifier::compareFunc( GLint func ) const
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, func );
    }

    void Texture2DModifier::wrap( GLenum xWrap, GLenum yWrap ) const
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, xWrap );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, yWrap );
    }

    void Texture2DModifier::anisotropicFiltering( bool enabled ) const
    {
#ifdef GL_TEXTURE_MAX_ANISOTROPY
        if( enabled )
        {
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16.0f );
        }
        else
        {
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 1.0f );
        }
#endif
    }

    void Texture2DModifier::generateMipMap() const
    {
        glGenerateMipmap( GL_TEXTURE_2D );
    }
}
