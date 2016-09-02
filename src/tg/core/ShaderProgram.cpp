#include "ShaderProgram.hpp"

#include <tg/core/VertexAttributeList.hpp>
#include <tg/core/VertexAttribute.hpp>
#include <tg/core/Shader.hpp>
#include <tg/math/Matrix4f.hpp>

#include <iostream>

tg::ShaderProgram::ShaderProgram(
    std::initializer_list< std::shared_ptr< Shader > > shaders,
    std::shared_ptr< VertexAttributeList > attributes )
    : m_programObject( glCreateProgram() )
{
    for( auto& shader : shaders )
        glAttachShader( m_programObject, shader->getShaderObject() );

    GLuint index = 0;
    for( auto& attribute : *attributes )
    {
        // DGL states that name has not to be terminated but c_str is - possible mistake here
        glBindAttribLocation( m_programObject, index, reinterpret_cast< const GLchar* >( attribute.getName().c_str() ) );
        ++index;
    }

    glLinkProgram( m_programObject );

    // check error log for info
    GLint linkStatus;
    glGetProgramiv( m_programObject, GL_LINK_STATUS, &linkStatus );

    if( linkStatus == GL_TRUE )
    {
        GLint size;
        glGetProgramiv( m_programObject, GL_INFO_LOG_LENGTH, &size );

        std::unique_ptr< GLchar[] > log( new GLchar[ size + 1 ] );
        glGetProgramInfoLog( m_programObject, size, NULL, log.get() );

        throw std::runtime_error( "Program error: " + std::string( log.get() ) );
    }
}

tg::ShaderProgram::~ShaderProgram()
{
    glDeleteProgram( m_programObject );
}

void tg::ShaderProgram::use() const
{
    glUseProgram( m_programObject );
}

void tg::ShaderProgram::unuse() const
{
    glUseProgram( 0 );
}

GLint tg::ShaderProgram::getUniformLocation( const std::string& name ) const
{
    return glGetUniformLocation( m_programObject, reinterpret_cast< const GLchar* >( name.c_str() ) );
}

void tg::ShaderProgram::setUniformInt( GLint location, GLint value )
{
    glUniform1i( location, value );
}

void tg::ShaderProgram::setUniform( GLint location, const tg::Matrix4f& matrix )
{
    glUniformMatrix4fv( location, 1, GL_TRUE, matrix.ptr() );
}

void tg::ShaderProgram::setUniform( GLint location, const tg::Vector< float, 3 >& vector )
{
    glUniform3f( location, vector.x(), vector.y(), vector.z() );
}
