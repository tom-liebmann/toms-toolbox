#pragma once

#include <GL/gl.h>

#include <memory>

namespace tgCore
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



    inline GLuint Shader::getShaderObject() const
    {
        return m_shaderObject;
    }
}
