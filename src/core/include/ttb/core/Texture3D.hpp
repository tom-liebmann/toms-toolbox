#pragma once

#include <GL/glew.h>

#include <GL/gl.h>

#include <cstdlib>

namespace ttb
{
    class Texture3D
    {
    public:
        Texture3D();

        ~Texture3D();

        void data( size_t width, size_t height, size_t depth, GLint internalFormat, GLenum format,
                   GLenum type, void* data );

        void minMagFilter( GLint minFilter, GLint magFilter );
        void wrap( GLenum xWrap, GLenum yWrap, GLenum zWrap );
        void borderColor( float r, float g, float b, float a );

        void bind( GLenum unit ) const;

        size_t width() const;
        size_t height() const;
        size_t depth() const;

    private:
        GLuint m_object;

        size_t m_width;
        size_t m_height;
        size_t m_depth;
    };
}
