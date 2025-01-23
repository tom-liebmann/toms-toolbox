#pragma once

#include "ShaderType.hpp"
#include <ttb/core/gl.hpp>

#include <memory>
#include <string_view>

// declarations
//=============================================================================

namespace ttb
{
    class Shader
    {
    public:
        static std::unique_ptr< Shader > fromFile( ShaderType type, std::string const& filename );

        static std::unique_ptr< Shader > fromSource( ShaderType type, std::string_view source );

        Shader( ShaderType type, std::string_view source );

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
