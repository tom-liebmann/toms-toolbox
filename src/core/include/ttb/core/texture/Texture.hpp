#pragma once

#include <GL/glew.h>

#include <GL/gl.h>

#include <cstdint>

namespace ttb
{
    class Texture
    {
    public:
        Texture();

        virtual ~Texture();

        GLuint object() const;

        virtual void bind( uint8_t slot ) = 0;

    protected:
        GLuint m_object;
    };
}
