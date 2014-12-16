#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <cstdint>
#include <memory>

namespace tgCore
{
    class Texture2D
    {
        public:
            static std::unique_ptr< Texture2D > fromPNG( const std::string& filename );

            Texture2D(
                uint16_t width,
                uint16_t height,
                GLint internalFormat,
                GLenum format,
                GLenum type );

            ~Texture2D();

            void setMinMagFilter( GLint minFilter, GLint magFilter );
            void setDepthTextureMode( GLint mode );
            void setTextureCompareMode( GLint mode );
            void setTextureCompareFunc( GLint func );

            uint16_t getWidth() const;
            uint16_t getHeight() const;
            GLuint getTextureObject() const;

            void bind( GLenum unit ) const;

        private:
            Texture2D();

            uint16_t m_width;
            uint16_t m_height;

            GLuint m_textureObject;
    };



    inline uint16_t Texture2D::getWidth() const
    {
        return m_width;
    }

    inline uint16_t Texture2D::getHeight() const
    {
        return m_height;
    }

    inline GLuint Texture2D::getTextureObject() const
    {
        return m_textureObject;
    }
}
