#pragma once

#include <ttb/core/gl.hpp>

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

        static std::unique_ptr< Shader > fromSource( Type type, std::string const& source );

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
