#pragma once

#include <GL/glew.h>

#include <GL/gl.h>

#include <ttb/core/shader/ShaderProgramCreator.hpp>
#include <ttb/math/Matrix.hpp>
#include <ttb/math/Vector.hpp>

#include <initializer_list>
#include <memory>

// forward declarations
//=============================================================================

namespace ttb
{
    class ShaderProgramCreator;
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
        static ShaderProgramCreator create();

        ~ShaderProgram();

        void use() const;
        void unuse() const;

        GLint getUniformLocation( const std::string& name ) const;
        void setUniformInt( GLint location, GLint value );
        void setUniform( GLint location, GLfloat value );
        void setUniform( GLint location, const ttb::Matrix4f& matrix );
        void setUniform( GLint location, const ttb::Vector< float, 3 >& vector );

    private:
        ShaderProgram( std::vector< std::unique_ptr< Shader > > const& shaders );

        GLuint m_programObject;

        friend class ShaderProgramCreator;
    };
}
