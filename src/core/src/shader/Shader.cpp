#include <ttb/core/shader/Shader.hpp>

#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

namespace ttb
{
    std::unique_ptr< Shader > Shader::fromFile( Type type, std::string const& filename )
    {
        std::ifstream stream( filename, std::ios::in | std::ios::binary );

        if( !stream.is_open() )
        {
            throw std::runtime_error( "Unable to open file " + filename );
        }

        // determine length of file
        stream.seekg( 0, std::ios::end );
        size_t size = stream.tellg();
        stream.seekg( 0, std::ios::beg );

        std::vector< char > buffer( size + 1 );

        memset( buffer.data(), 0, size + 1 );

        stream.read( buffer.data(), size );

        buffer[ size ] = '\0';

        try
        {
            auto shader = std::unique_ptr< Shader >(
                new Shader( type, std::string( std::begin( buffer ), std::end( buffer ) ) ) );

            return shader;
        }
        catch( std::runtime_error& e )
        {
            throw std::runtime_error( "Error compiling shader (" + filename + "): " + e.what() );
        }
    }

    Shader::Shader( Type type, std::string const& source )
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

        auto sourcePtr = source.c_str();  // We need to provide an l-value
        glShaderSource( m_shaderObject, 1, reinterpret_cast< GLchar const** >( &sourcePtr ), NULL );

        glCompileShader( m_shaderObject );

        // check error log for info
        GLint compileStatus;
        glGetShaderiv( m_shaderObject, GL_COMPILE_STATUS, &compileStatus );

        if( !compileStatus )
        {
            GLchar buffer[ 256 ];
            glGetShaderInfoLog( m_shaderObject, 256, NULL, buffer );

            throw std::runtime_error( "Shader error: " + std::string( buffer ) );
        }
    }

    Shader::~Shader()
    {
        glDeleteShader( m_shaderObject );
    }
}
