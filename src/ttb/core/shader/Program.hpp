#pragma once

#include <GL/glew.h>

#include <GL/gl.h>

#include <ttb/math/Matrix.hpp>
#include <ttb/math/Vector.hpp>

#include <initializer_list>
#include <memory>
#include <vector>

// forward declarations
//=============================================================================

namespace ttb
{
    class ProgramCreator;
    class Shader;
    class VertexAttributeList;
}



// declarations
//=============================================================================

namespace ttb
{
    class Program
    {
        class Creator;

    public:
        static Creator create();

        ~Program();

        void use() const;
        void unuse() const;

        GLint attributeLocation( std::string const& name ) const;

        GLint getUniformLocation( const std::string& name ) const;
        void setUniformInt( GLint location, GLint value );
        void setUniform( GLint location, GLfloat value );
        void setUniform( GLint location, const ttb::Matrix4f& matrix );
        void setUniform( GLint location, const ttb::Vector< float, 3 >& vector );

    private:
        Program( std::vector< std::unique_ptr< Shader > > const& shaders );

        GLuint m_programObject;
    };



    class Program::Creator
    {
    public:
        Creator& attachShader( std::unique_ptr< Shader > shader );

        std::unique_ptr< Program > finish();

    private:
        Creator();

        std::vector< std::unique_ptr< Shader > > m_shaders;

        friend class Program;
    };
}
