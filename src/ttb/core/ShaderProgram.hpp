#pragma once

#include <GL/glew.h>

#include <GL/gl.h>

#include <ttb/math/Vector.hpp>
#include <ttb/math/Matrix.hpp>

#include <initializer_list>
#include <memory>

// forward declarations
//=============================================================================

namespace ttb
{
    class Shader;
    class VertexAttributeList;
}



// declarations
//=============================================================================

namespace ttb
{
    class ShaderProgram
    {
    public:
        ShaderProgram( std::initializer_list< std::shared_ptr< Shader > > shaders,
                       std::shared_ptr< VertexAttributeList > attributes );

        ~ShaderProgram();

        void use() const;
        void unuse() const;

        // uniform handling
        GLint getUniformLocation( const std::string& name ) const;

        void setUniformInt( GLint location, GLint value );
        void setUniform( GLint location, const ttb::Matrix4f& matrix );
        void setUniform( GLint location, const ttb::Vector< float, 3 >& vector );

    private:
        GLuint m_programObject;
    };
}
