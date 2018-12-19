#pragma once

#include <ttb/core/gl.hpp>

#include <cstdint>

namespace ttb
{
    class Texture
    {
    public:
        Texture();

        virtual ~Texture();

        GLuint object() const;

        virtual void bind( uint8_t slot ) const = 0;

    protected:
        GLuint m_object;
    };
}
