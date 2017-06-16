#include <ttb/core/texture/Texture.hpp>

#include <cstdint>

namespace ttb
{
    Texture::Texture()
    {
        glGenTextures( 1, &m_object );
    }

    Texture::~Texture()
    {
        glDeleteTextures( 1, &m_object );
    }

    GLuint Texture::object() const
    {
        return m_object;
    }
}
