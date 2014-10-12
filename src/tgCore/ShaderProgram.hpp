#pragma once

#include <GL/gl.h>

#include <initializer_list>
#include <memory>

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

        private:
            GLuint m_programObject;
    };
}
