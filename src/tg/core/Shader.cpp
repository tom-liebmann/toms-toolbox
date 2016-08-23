#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <cstring>

using namespace tg;

std::shared_ptr< tg::Shader > tg::Shader::fromFile( Type type, const std::string& filename )
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

    return shader;
}

tg::Shader::Shader( Type type, const char* code )
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
        GLchar* log = new GLchar[ size + 1 ];
        glGetShaderInfoLog( m_shaderObject, size, NULL, log );

        std::cout << "################ CODE #################" << std::endl;
        std::cout << code << std::endl;
        std::cout << "################ ERROR ################" << std::endl;
        std::cout << log << std::endl;

        delete[] log;
    }
}

tg::Shader::~Shader()
{
    glDeleteShader( m_shaderObject );
}
