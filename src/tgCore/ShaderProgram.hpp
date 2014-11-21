#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <initializer_list>
#include <memory>

namespace tgMath
{
    class Matrix4f;
}

namespace tgCore
{
    class Shader;
    class VertexAttributeList;

    class ShaderProgram
    {
        public:
            ShaderProgram(
                std::initializer_list< std::shared_ptr< Shader > > shaders,
                std::shared_ptr< VertexAttributeList > attributes );

            ~ShaderProgram();

            void use() const;
            void unuse() const;

            // uniform handling
            GLint getUniformLocation( const std::string& name ) const;

            void setUniformInt( GLint location, GLint value );
            void setUniform( GLint location, const tgMath::Matrix4f& matrix );

        private:
            GLuint m_programObject;
    };
}
