#include <ttb/core/texture/Texture2D.hpp>

#include <cassert>
#include <iostream>
#include <utility>

namespace ttb
{
    Texture2D::Modifier Texture2D::create(
        size_t width, size_t height, GLint internalFormat, uint8_t levels, uint8_t samples )
    {
        return modify( std::shared_ptr< Texture2D >(
            new Texture2D( width, height, internalFormat, levels, samples ) ) );
    }

    size_t Texture2D::width() const
    {
        return m_width;
    }

    size_t Texture2D::height() const
    {
        return m_height;
    }

    uint8_t Texture2D::colorChannel() const
    {
        switch( m_internalFormat )
        {
            case GL_R32F:
                return 1;

            case GL_RGB:
            case GL_RGB32F:
                return 3;

            case GL_RGBA:
            case GL_RGBA8:
            case GL_RGBA32F:
                return 4;

            default:
                throw std::runtime_error( "Unknown pixel format (" +
                                          std::to_string( m_internalFormat ) + ")" );
        }
    }

    uint8_t Texture2D::samples() const
    {
        return m_samples;
    }

    void Texture2D::bind( uint8_t slot ) const
    {
        glActiveTexture( GL_TEXTURE0 + slot );
        glBindTexture( target(), object() );
    }

    void Texture2D::unbind( uint8_t slot ) const
    {
        glActiveTexture( GL_TEXTURE0 + slot );
        glBindTexture( target(), 0 );
    }


    Texture2D::Texture2D(
        size_t width, size_t height, GLint internalFormat, uint8_t levels, uint8_t samples )
        : m_width( width )
        , m_height( height )
        , m_internalFormat( internalFormat )
        , m_levels{ levels }
        , m_samples{ samples }
    {
        bind( 0 );
        initStorage();
        unbind( 0 );
    }

    GLenum Texture2D::target() const
    {
        return m_samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    void Texture2D::download( GLint level, std::vector< std::uint8_t >& buffer ) const
    {
#ifndef PLATFORM_ANDROID
        std::size_t bytesPerPixel;
        GLenum format;
        GLenum outType;
        switch( m_internalFormat )
        {
            case GL_R32UI:
                bytesPerPixel = 4;
                format = GL_RED_INTEGER;
                outType = GL_UNSIGNED_INT;
                break;

            case GL_R32F:
                bytesPerPixel = 4;
                format = GL_RED;
                outType = GL_FLOAT;
                break;

            case GL_RGB:
                bytesPerPixel = 3;
                format = GL_RGB;
                outType = GL_UNSIGNED_BYTE;
                break;

            case GL_RGBA:
            case GL_RGBA8:
                bytesPerPixel = 4;
                format = GL_RGBA;
                outType = GL_UNSIGNED_BYTE;
                break;

            case GL_RGBA32F:
                bytesPerPixel = 16;
                format = GL_RGBA;
                outType = GL_FLOAT;

            default:
                throw std::runtime_error( "Unknown pixel format (" +
                                          std::to_string( m_internalFormat ) + ")" );
        }

        buffer.resize( m_width * m_height * bytesPerPixel );
        glGetTextureImage( object(), level, format, outType, buffer.size(), buffer.data() );
#endif
    }

    void Texture2D::initStorage()
    {
#if defined( PLATFORM_ANDROID )
        glTexImage2D( GL_TEXTURE_2D,
                      0,
                      m_internalFormat,
                      m_width,
                      m_height,
                      0,
                      GL_RGBA,
                      GL_UNSIGNED_BYTE,
                      nullptr );
#else
        if( m_samples > 1 )
        {
            glTexStorage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE,
                                       m_samples,
                                       m_internalFormat,
                                       m_width,
                                       m_height,
                                       GL_TRUE );
        }
        else
        {
            glTexStorage2D( GL_TEXTURE_2D, m_levels, m_internalFormat, m_width, m_height );
        }
#endif
    }


    Texture2D::Modifier::~Modifier()
    {
        if( m_texture )
        {
            std::cerr << "Texture still in modification state" << std::endl;
        }
    }

    Texture2D::Modifier& Texture2D::Modifier::resize(
        size_t width, size_t height, GLint internalFormat, uint8_t levels, uint8_t samples )
    {
        m_texture->m_width = width;
        m_texture->m_height = height;
        m_texture->m_internalFormat = internalFormat;
        m_texture->m_levels = levels;
        m_texture->m_samples = samples;

        m_texture->unbind( 0 );

        glDeleteTextures( 1, &m_texture->m_object );
        glGenTextures( 1, &m_texture->m_object );

        m_texture->bind( 0 );
        m_texture->initStorage();

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::upload( size_t x,
                                                      size_t y,
                                                      size_t width,
                                                      size_t height,
                                                      GLenum format,
                                                      GLenum valueType,
                                                      void const* data )
    {
        assert( m_texture->m_samples == 1 );

        glTexSubImage2D( m_texture->target(), 0, x, y, width, height, format, valueType, data );

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::upload( size_t width,
                                                      size_t height,
                                                      GLint internalFormat,
                                                      GLenum format,
                                                      GLenum valueType,
                                                      void const* data )
    {
        resize( width, height, internalFormat );
        upload( format, valueType, data );

        return *this;
    }

    Texture2D::Modifier&
        Texture2D::Modifier::upload( GLenum format, GLenum valueType, void const* data )
    {
        return upload( 0, 0, m_texture->m_width, m_texture->m_height, format, valueType, data );
    }

    Texture2D::Modifier&
        Texture2D::Modifier::download( [[maybe_unused]] size_t level,
                                       [[maybe_unused]] std::vector< uint8_t >& buffer )
    {
#ifndef PLATFORM_ANDROID
        GLint width, height;
        glGetTexLevelParameteriv( GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &width );
        glGetTexLevelParameteriv( GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &height );

        buffer.resize( width * height * m_texture->colorChannel() );

        glGetTexImage( GL_TEXTURE_2D, level, GL_RGB, GL_UNSIGNED_BYTE, buffer.data() );
#endif

        return *this;
    }

    Texture2D::Modifier&
        Texture2D::Modifier::download( [[maybe_unused]] size_t level,
                                       [[maybe_unused]] std::vector< float >& buffer )
    {
#ifndef PLATFORM_ANDROID
        GLint width, height;
        glGetTexLevelParameteriv( GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &width );
        glGetTexLevelParameteriv( GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &height );

        buffer.resize( width * height * m_texture->colorChannel() );

        glGetTexImage( GL_TEXTURE_2D, level, GL_RGB, GL_FLOAT, buffer.data() );
#endif

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::minMagFilter( GLint minFilter, GLint magFilter )
    {
        glTexParameteri( m_texture->target(), GL_TEXTURE_MIN_FILTER, minFilter );
        glTexParameteri( m_texture->target(), GL_TEXTURE_MAG_FILTER, magFilter );

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::depthMode( [[maybe_unused]] GLint mode )
    {
#ifndef PLATFORM_ANDROID

        glTexParameteri( m_texture->target(), GL_DEPTH_TEXTURE_MODE, mode );

#endif

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::compareMode( GLint mode )
    {
        glTexParameteri( m_texture->target(), GL_TEXTURE_COMPARE_MODE, mode );

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::compareFunc( GLint func )
    {
        glTexParameteri( m_texture->target(), GL_TEXTURE_COMPARE_FUNC, func );

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::wrap( GLenum xWrap, GLenum yWrap )
    {
        glTexParameteri( m_texture->target(), GL_TEXTURE_WRAP_S, xWrap );
        glTexParameteri( m_texture->target(), GL_TEXTURE_WRAP_T, yWrap );

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::anisotropicFiltering( [[maybe_unused]] bool enabled )
    {
#ifdef GL_TEXTURE_MAX_ANISOTROPY
        if( enabled )
        {
            glTexParameterf( m_texture->target(), GL_TEXTURE_MAX_ANISOTROPY, 16.0f );
        }
        else
        {
            glTexParameterf( m_texture->target(), GL_TEXTURE_MAX_ANISOTROPY, 1.0f );
        }
#endif

        return *this;
    }

    Texture2D::Modifier& Texture2D::Modifier::generateMipMap()
    {
        assert( m_texture->m_samples == 1 );

        glGenerateMipmap( m_texture->target() );

        return *this;
    }

    std::shared_ptr< Texture2D > Texture2D::Modifier::finish()
    {
        m_texture->unbind( 0 );

        return std::exchange( m_texture, std::shared_ptr< Texture2D >() );
    }

    Texture2D::Modifier::Modifier( std::shared_ptr< Texture2D > texture )
        : m_texture( std::move( texture ) )
    {
        m_texture->bind( 0 );
    }


    Texture2D::ConstModifier::~ConstModifier()
    {
        if( m_texture )
        {
            std::cerr << "Texture still in modification state" << std::endl;
        }
    }

    Texture2D::ConstModifier&
        Texture2D::ConstModifier::download( [[maybe_unused]] size_t level,
                                            [[maybe_unused]] std::vector< uint8_t >& buffer )
    {
        // #ifndef PLATFORM_ANDROID

        //         GLint width, height;
        //         glGetTexLevelParameteriv( GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &width );
        //         glGetTexLevelParameteriv( GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &height );

        //         buffer.resize( width * height * m_texture->bytesPerPixel() );

        //         glGetTexImage(
        //             GL_TEXTURE_2D, level, m_texture->m_format, m_texture->m_valueType,
        //             buffer.data() );

        // #endif

        return *this;
    }

    Texture2D::ConstModifier&
        Texture2D::ConstModifier::anisotropicFiltering( [[maybe_unused]] bool enabled )
    {
#ifdef GL_TEXTURE_MAX_ANISOTROPY
        if( enabled )
        {
            glTexParameterf( m_texture->target(), GL_TEXTURE_MAX_ANISOTROPY, 16.0f );
        }
        else
        {
            glTexParameterf( m_texture->target(), GL_TEXTURE_MAX_ANISOTROPY, 1.0f );
        }
#endif

        return *this;
    }

    void Texture2D::ConstModifier::finish()
    {
        m_texture->unbind( 0 );
        m_texture = nullptr;
    }

    Texture2D::ConstModifier::ConstModifier( ttb::Texture2D const& texture ) : m_texture( &texture )
    {
        m_texture->bind( 0 );
    }


    Texture2D::Modifier modify( std::shared_ptr< Texture2D > texture )
    {
        return { std::move( texture ) };
    }

    Texture2D::ConstModifier modify( Texture2D const& texture )
    {
        return { texture };
    }
}
