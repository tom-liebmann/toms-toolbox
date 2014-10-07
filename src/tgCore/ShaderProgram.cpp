#include <GL/glew.h>

#include "ShaderProgram.hpp"

#include <tgCore/VertexAttributeList.hpp>
#include <tgCore/VertexAttribute.hpp>
#include <tgCore/Shader.hpp>

using tgCore::ShaderProgram;

ShaderProgram::ShaderProgram(
    std::initializer_list< Shader > shaders,
    std::shared_ptr< VertexAttributeList > attributes )
    : m_programObject( glCreateProgram() )
{
    for( auto& shader : shaders )
    {
        glAttachShader( m_programObject, shader.getShaderObject() );
    }

    GLuint index = 0;
    for( auto& attribute : *attributes.get() )
    {
        // DGL states that name has not to be terminated but c_str is - possible mistake here
        glBindAttribLocation( m_programObject, index, reinterpret_cast< const GLchar* >( attribute.getName().c_str() ) );
        ++index;
    }

    glLinkProgram( m_programObject );

    // TODO: check for errors
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram( m_programObject );
}

void ShaderProgram::use() const
{
    static GLuint usedProgram = 0;

    if( usedProgram != m_programObject )
    {
        glUseProgram( m_programObject );
        usedProgram = m_programObject;
    }
}

void ShaderProgram::unuse() const
{
    glUseProgram( 0 );
}

GLint ShaderProgram::getUniformLocation( const std::string& name ) const
{
    return glGetUniformLocation( m_programObject, reinterpret_cast< const GLchar* >( name.c_str() ) );
}
