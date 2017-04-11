#include "ShaderProgram.hpp"

#include <ttb/core/shader/Shader.hpp>
#include <ttb/core/VertexAttribute.hpp>
#include <ttb/core/VertexAttributeList.hpp>
#include <ttb/math/Matrix4f.hpp>

#include <iostream>

ttb::ShaderProgram::ShaderProgram( std::initializer_list< std::shared_ptr< Shader > > shaders,
                                   std::shared_ptr< VertexAttributeList > attributes )
    : m_programObject( glCreateProgram() )
{
    for( auto& shader : shaders )
        glAttachShader( m_programObject, shader->getShaderObject() );

    GLuint index = 0;
    for( auto& attribute : *attributes )
    {
        // DGL states that name has not to be terminated but c_str is - possible mistake here
        glBindAttribLocation( m_programObject, index,
                              reinterpret_cast< const GLchar* >( attribute.getName().c_str() ) );
        ++index;
    }

    glLinkProgram( m_programObject );

    // check error log for info
    GLint linkStatus;
    glGetProgramiv( m_programObject, GL_LINK_STATUS, &linkStatus );

    if( !linkStatus )
    {
        GLchar buffer[ 256 ];
        glGetProgramInfoLog( m_programObject, 256, NULL, buffer );

        throw std::runtime_error( "Program error: " + std::string( buffer ) );
    }
}

ttb::ShaderProgram::~ShaderProgram()
{
    glDeleteProgram( m_programObject );
}

void ttb::ShaderProgram::use() const
{
    glUseProgram( m_programObject );
}

void ttb::ShaderProgram::unuse() const
{
    glUseProgram( 0 );
}

GLint ttb::ShaderProgram::getUniformLocation( const std::string& name ) const
{
    return glGetUniformLocation( m_programObject,
                                 reinterpret_cast< const GLchar* >( name.c_str() ) );
}

void ttb::ShaderProgram::setUniformInt( GLint location, GLint value )
{
    glUniform1i( location, value );
}

void ttb::ShaderProgram::setUniform( GLint location, GLfloat value )
{
    glUniform1f( location, value );
}

void ttb::ShaderProgram::setUniform( GLint location, const ttb::Matrix4f& matrix )
{
    glUniformMatrix4fv( location, 1, GL_TRUE, matrix.ptr() );
}

void ttb::ShaderProgram::setUniform( GLint location, const ttb::Vector< float, 3 >& vector )
{
    glUniform3f( location, vector.x(), vector.y(), vector.z() );
}
