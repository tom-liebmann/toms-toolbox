#pragma once

#include <ttb/core/gl.hpp>
#include <ttb/core/texture/Texture.hpp>

#include <cstdlib>
#include <memory>

namespace ttb
{
    class Texture3DModifier;


    class Texture3D : public Texture
    {
        friend class Texture3DModifier;

    public:
        Texture3D();

        std::unique_ptr< Texture3DModifier > modify();

        uint16_t width() const;

        uint16_t height() const;

        uint16_t depth() const;

        // Override: Texture
        virtual void bind( uint8_t slot ) const override;

        virtual void unbind( uint8_t slot ) const override;

    private:
        uint16_t m_width;

        uint16_t m_height;

        uint16_t m_depth;

        GLint m_internalFormat;

        GLenum m_format;

        GLenum m_valueType;
    };


    class Texture3DModifier
    {
        friend class Texture3D;

    public:
        ~Texture3DModifier();

        void upload( uint16_t width,
                     uint16_t height,
                     uint16_t depth,
                     GLint internalFormat,
                     GLenum format,
                     GLenum valueType,
                     void const* data ) const;

        void minMagFilter( GLint minFilter, GLint magFilter ) const;

        void wrap( GLenum xWrap, GLenum yWrap, GLenum zWrap ) const;

        void borderColor( float r, float g, float b, float a ) const;

    private:
        Texture3DModifier( Texture3D& texture );

        Texture3D& m_texture;
    };
}
