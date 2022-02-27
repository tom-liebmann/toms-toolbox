#include <ttb/core/shader/Shader.hpp>

#include <ttb/core/gl.hpp>

#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

namespace ttb
{
#if !defined( PLATFORM_ANDROID )
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
#endif

    std::unique_ptr< Shader > Shader::fromSource( Type type, std::string const& source )
    {
        return std::unique_ptr< Shader >( new Shader( type, source ) );
    }

    Shader::Shader( Type type, std::string const& source )
    {
        // create shader object
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
        }

        if( 0 == m_shaderObject )
        {
            throw std::runtime_error( "Unable to create shader object" );
        }

        // load source into shader
        {
            std::array< GLchar const*, 1 > sources = { { source.c_str() } };
            glShaderSource( m_shaderObject, 1, sources.data(), nullptr );
        }

        // compile shader
        {
            glCompileShader( m_shaderObject );

            // check error log for info
            GLint compileStatus = GL_FALSE;
            glGetShaderiv( m_shaderObject, GL_COMPILE_STATUS, &compileStatus );

            if( compileStatus != GL_TRUE )
            {
                auto const errorMsg = [ & ]() -> std::string
                {
                    std::array< GLchar, 256 > buffer;
                    GLsizei strLength = 0;
                    glGetShaderInfoLog( m_shaderObject, buffer.size(), &strLength, buffer.data() );

                    if( strLength > 0 )
                    {
                        return "Shader error: " + std::string( buffer.data() );
                    }
                    else
                    {
                        return "No error message";
                    }
                }();

                throw std::runtime_error( errorMsg );
            }
        }
    }

    Shader::~Shader()
    {
        glDeleteShader( m_shaderObject );
    }
}
