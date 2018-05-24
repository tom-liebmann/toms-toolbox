#include <ttb/core/texture/Texture2D.hpp>

#include <iostream>

namespace ttb
{
    Texture2D::Modifier Texture2D::create()
    {
        return modify( std::shared_ptr< Texture2D >( new Texture2D() ) );
    }

    Texture2D::Modifier Texture2D::create(
        size_t width, size_t height, GLint internalFormat, GLenum format, GLenum valueType )
    {
        return modify( std::shared_ptr< Texture2D >(
            new Texture2D( width, height, internalFormat, format, valueType ) ) );
    }

    size_t Texture2D::width() const
    {
        return m_width;
    }

    size_t Texture2D::height() const
    {
        return m_height;
    }

    uint8_t Texture2D::bytesPerPixel() const
    {
        switch( m_internalFormat )
        {
            case GL_RGBA:
            case GL_R32F:
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

    Texture2D::Texture2D() : m_width( 0 ), m_height( 0 )
    {
    }

    Texture2D::Texture2D(
        size_t width, size_t height, GLint internalFormat, GLenum format, GLenum valueType )
        : m_width( width )
        , m_height( height )
        , m_internalFormat( internalFormat )
        , m_format( format )
        , m_valueType( valueType )
    {
        glBindTexture( GL_TEXTURE_2D, m_object );

        glTexImage2D(
            GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, m_valueType, NULL );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        glBindTexture( GL_TEXTURE_2D, 0 );
    }


    Texture2D::Modifier::~Modifier()
    {
        if( m_texture )
        {
            std::cerr << "Texture still in modification state" << std::endl;
        }
    }

    Texture2D::Modifier& Texture2D::Modifier::upload(
        size_t x, size_t y, size_t width, size_t height, void const* data )
    {
        glTexSubImage2D( GL_TEXTURE_2D,
                         0,
                         x,
                         y,
                         width,
                         height,
                         m_texture->m_format,
                         m_texture->m_valueType,
                         data );

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::upload( size_t width,
                                                      size_t height,
                                                      GLint internalFormat,
                                                      GLenum format,
                                                      GLenum valueType,
                                                      void const* data )
    {
        m_texture->m_width = width;
        m_texture->m_height = height;
        m_texture->m_internalFormat = internalFormat;
        m_texture->m_format = format;
        m_texture->m_valueType = valueType;

        glTexImage2D( GL_TEXTURE_2D,
                      0,
                      m_texture->m_internalFormat,
                      m_texture->m_width,
                      m_texture->m_height,
                      0,
                      m_texture->m_format,
                      m_texture->m_valueType,
                      data );

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::upload( void const* data )
    {
        glTexSubImage2D( GL_TEXTURE_2D,
                         0,
                         0,
                         0,
                         m_texture->m_width,
                         m_texture->m_height,
                         m_texture->m_format,
                         m_texture->m_valueType,
                         data );

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::download( size_t level,
                                                        std::vector< uint8_t >& buffer )
    {
        GLint width, height;
        glGetTexLevelParameteriv( GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &width );
        glGetTexLevelParameteriv( GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &height );

        buffer.resize( width * height * m_texture->bytesPerPixel() );

        glGetTexImage(
            GL_TEXTURE_2D, level, m_texture->m_format, m_texture->m_valueType, buffer.data() );

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::minMagFilter( GLint minFilter, GLint magFilter )
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter );

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::depthMode( GLint mode )
    {
        glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, mode );

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::compareMode( GLint mode )
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, mode );

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::compareFunc( GLint func )
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, func );

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::wrap( GLenum xWrap, GLenum yWrap )
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, xWrap );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, yWrap );

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::anisotropicFiltering( bool enabled )
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

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::generateMipMap()
    {
        glGenerateMipmap( GL_TEXTURE_2D );

        return *this;
    }

    std::shared_ptr< Texture2D > Texture2D::Modifier::finish()
    {
        glBindTexture( GL_TEXTURE_2D, 0 );

        return std::exchange( m_texture, std::shared_ptr< Texture2D >() );
    }

    Texture2D::Modifier::Modifier( std::shared_ptr< Texture2D > texture )
        : m_texture( std::move( texture ) )
    {
        glBindTexture( GL_TEXTURE_2D, m_texture->object() );
    }


    Texture2D::Modifier modify( std::shared_ptr< Texture2D > texture )
    {
        return { std::move( texture ) };
    }
}
