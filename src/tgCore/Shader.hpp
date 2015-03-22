#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <memory>

// declarations
//=============================================================================

namespace tg
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

            static std::shared_ptr< Shader > fromFile(
                Type type, const std::string& filename );

            ~Shader();

            GLuint getShaderObject() const;

        private:
            Shader( Type type, const char* code );

            GLuint m_shaderObject;
    };
}



// definitions
//=============================================================================

namespace tg
{
    inline GLuint Shader::getShaderObject() const
    {
        return m_shaderObject;
    }
}
