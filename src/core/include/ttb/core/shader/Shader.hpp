#pragma once

#include <GL/glew.h>

#include <GL/gl.h>

#include <memory>

// declarations
//=============================================================================

namespace ttb
{
    class Shader
    {
    public:
        enum class Type
        {
            VERTEX,
            FRAGMENT,
            GEOMETRY
        };

        static std::unique_ptr< Shader > fromFile( Type type, std::string const& filename );

        Shader( Type type, std::string const& source );

        ~Shader();

        GLuint shaderObject() const;

    private:
        GLuint m_shaderObject;
    };
}



// definitions
//=============================================================================

namespace ttb
{
    inline GLuint Shader::shaderObject() const
    {
        return m_shaderObject;
    }
}
