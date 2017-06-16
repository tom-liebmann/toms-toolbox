#pragma once

#include <GL/glew.h>

#include <GL/gl.h>

#include <ttb/core/texture/Texture.hpp>

#include <cstdint>
#include <memory>

// declarations
//=============================================================================

namespace ttb
{
    class Texture2DModifier;


    class Texture2D : public Texture
    {
        friend class Texture2DModifier;

    public:
        Texture2D();

        Texture2D( uint16_t width,
                   uint16_t height,
                   GLint internalFormat,
                   GLenum format,
                   GLenum valueType );

        std::unique_ptr< Texture2DModifier > modify();

        // Properties
        uint16_t width() const;

        uint16_t height() const;

    private:
        uint16_t m_width;
        uint16_t m_height;
        GLint m_internalFormat;
        GLenum m_format;
        GLenum m_valueType;
    };


    class Texture2DModifier
    {
        friend class Texture2D;

    public:
        ~Texture2DModifier();

        /// Update a rectangular part of the texture
        void upload( uint16_t x, uint16_t y, uint16_t width, uint16_t height, void const* data ) const;

        /// Upload values into the texture
        /*
        * If dimensions do not match, the texture gets resized.
        */
        void upload( uint16_t width,
                     uint16_t height,
                     GLint internalFormat,
                     GLenum format,
                     GLenum valueType,
                     void const* data ) const;

        /// Update the textures data
        void upload( void const* data ) const;

        void minMagFilter( GLint minFilter, GLint magFilter ) const;

        void depthMode( GLint mode ) const;

        void compareMode( GLint mode ) const;

        void compareFunc( GLint func ) const;

        void wrap( GLenum xWrap, GLenum yWrap ) const;

    private:
        Texture2DModifier( Texture2D& texture );

        Texture2D& m_texture;
    };
}
