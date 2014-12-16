#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <cstring>

using tgCore::Shader;

std::shared_ptr< Shader > Shader::fromFile( Type type, const std::string& filename )
{
    std::ifstream stream( filename, std::ios::in | std::ios::binary );

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

    return std::move( shader );
}

Shader::Shader( Type type, const char* code )
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
    GLint size;
    glGetShaderiv( m_shaderObject, GL_INFO_LOG_LENGTH, &size );

    if( size > 1 )
    {
        GLchar* log = new GLchar[ size + 1 ];
        glGetShaderInfoLog( m_shaderObject, size, NULL, log );

        std::cout << "################ ERROR ################" << std::endl;
        std::cout << code << std::endl;
        std::cout << "################ CODE #################" << std::endl;
        std::cout << log << std::endl;

        delete[] log;
    }
}

Shader::~Shader()
{
    glDeleteShader( m_shaderObject );
}
