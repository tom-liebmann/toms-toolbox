#pragma once

#include <GL/glew.h>

#include <GL/gl.h>

#include <ttb/core/texture/Texture.hpp>

#include <cstdint>
#include <memory>
#include <vector>


// declarations
//=============================================================================

namespace ttb
{
    class Texture2D : public Texture
    {
    public:
        class Modifier;

        static Modifier create();

        static Modifier create(
            size_t width, size_t height, GLint internalFormat, GLenum format, GLenum valueType );

        // Properties
        size_t width() const;

        size_t height() const;

        uint8_t colorChannel() const;

        uint8_t bytesPerPixel() const;

        uint8_t bitDepth() const;

        // Override: Texture
        virtual void bind( uint8_t slot ) const override;

    private:
        Texture2D();

        Texture2D(
            size_t width, size_t height, GLint internalFormat, GLenum format, GLenum valueType );

        size_t m_width;
        size_t m_height;
        GLint m_internalFormat;
        GLenum m_format;
        GLenum m_valueType;
    };


    class Texture2D::Modifier
    {
    public:
        ~Modifier();

        /// Upload values into the texture
        /*
         * If dimensions do not match, the texture gets resized.
         */
        Modifier& upload( size_t width,
                          size_t height,
                          GLint internalFormat,
                          GLenum format,
                          GLenum valueType,
                          void const* data );

        /// Update a rectangular part of the texture
        Modifier& upload( size_t x, size_t y, size_t width, size_t height, void const* data );

        /// Update the texture's data
        Modifier& upload( void const* data );

        Modifier& download( size_t level, std::vector< uint8_t >& buffer );

        Modifier& minMagFilter( GLint minFilter, GLint magFilter );

        Modifier& depthMode( GLint mode );

        Modifier& compareMode( GLint mode );

        Modifier& compareFunc( GLint func );

        Modifier& wrap( GLenum xWrap, GLenum yWrap );

        Modifier& anisotropicFiltering( bool enabled );

        Modifier& generateMipMap();

        std::shared_ptr< Texture2D > finish();

    private:
        Modifier( std::shared_ptr< Texture2D > texture );

        std::shared_ptr< Texture2D > m_texture;

        friend Modifier modify( std::shared_ptr< Texture2D > texture );
    };


    Texture2D::Modifier modify( std::shared_ptr< Texture2D > texture );
}
