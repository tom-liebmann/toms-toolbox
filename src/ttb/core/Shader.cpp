#include "Shader.hpp"

#include <cstring>
#include <fstream>
#include <iostream>

using namespace ttb;

std::shared_ptr< ttb::Shader > ttb::Shader::fromFile( Type type, const std::string& filename )
{
    std::ifstream stream( filename, std::ios::in | std::ios::binary );

    if( !stream.is_open() )
        throw std::runtime_error( "Unable to open file " + filename );

    // determine length of file
    stream.seekg( 0, std::ios::end );
    size_t size = stream.tellg();
    stream.seekg( 0, std::ios::beg );

    char* buffer = new char[ size + 1 ];

    memset( buffer, 0, size + 1 );

    stream.read( buffer, size );

    buffer[ size ] = '\0';

    auto shader = std::shared_ptr< Shader >( new Shader( type, buffer ) );

    stream.close();
    delete[] buffer;

    return shader;
}

ttb::Shader::Shader( Type type, const char* code )
{
    switch( type )
    {
        case Type::VERTEX:
            m_shaderObject = glCreateShader( GL_VERTEX_SHADER );
            break;
        case Type::FRAGMENT:
            m_shaderObject = glCreateShader( GL_FRAGMENT_SHADER );
            break;
        case Type::GEOMETRY:
            m_shaderObject = glCreateShader( GL_GEOMETRY_SHADER );
            break;
    }

    glShaderSource( m_shaderObject, 1, reinterpret_cast< const GLchar** >( &code ), NULL );

    glCompileShader( m_shaderObject );

    // check error log for info
    GLint compileStatus;
    glGetShaderiv( m_shaderObject, GL_COMPILE_STATUS, &compileStatus );

    if( compileStatus != GL_TRUE )
    {
        GLint size;
        glGetShaderiv( m_shaderObject, GL_INFO_LOG_LENGTH, &size );

        std::unique_ptr< GLchar[] > log( new GLchar[ size + 1 ] );
        glGetShaderInfoLog( m_shaderObject, size, NULL, log.get() );

        throw std::runtime_error( "Shader error: " + std::string( log.get() ) );
    }
}

ttb::Shader::~Shader()
{
    glDeleteShader( m_shaderObject );
}
