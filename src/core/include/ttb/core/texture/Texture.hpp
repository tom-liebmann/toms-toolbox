#pragma once

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

    protected:
        GLuint m_object;
    };
}
