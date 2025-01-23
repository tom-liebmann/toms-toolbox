#include "ttb/core/shader/ShaderType.hpp"
#include <ttb/core/shader/Shader.hpp>

#include <ttb/core/gl.hpp>

#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>


namespace
{
    auto createShaderObject( ttb::ShaderType type ) -> GLuint;

    void loadShaderSource( GLuint shaderObject, std::string_view source );

    void compileShader( GLuint shaderObject );

    auto getInfoLog( GLuint shaderObject ) -> std::string;
}

namespace ttb
{
#if !defined( PLATFORM_ANDROID )
    std::unique_ptr< Shader > Shader::fromFile( ShaderType type, std::string const& filename )
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
                new Shader( type, { std::begin( buffer ), std::end( buffer ) } ) );

            return shader;
        }
        catch( std::runtime_error& e )
        {
            throw std::runtime_error( "Error compiling shader (" + filename + "): " + e.what() );
        }
    }
#endif

    std::unique_ptr< Shader > Shader::fromSource( ShaderType type, std::string_view source )
    {
        return std::unique_ptr< Shader >( new Shader( type, source ) );
    }

    Shader::Shader( ShaderType type, std::string_view source )
    {
        m_shaderObject = createShaderObject( type );

        loadShaderSource( m_shaderObject, source );

        compileShader( m_shaderObject );
    }

    Shader::~Shader()
    {
        glDeleteShader( m_shaderObject );
    }
}


namespace
{
    auto createShaderObject( ttb::ShaderType type ) -> GLuint
    {
        if( auto const shaderObject = glCreateShader( convertToGl( type ) ); shaderObject != 0 )
        {
            return shaderObject;
        }
        else
        {
            throw std::runtime_error( "Unable to create shader object" );
        }
    }

    void loadShaderSource( GLuint shaderObject, std::string_view source )
    {
        auto const sources = std::array< GLchar const*, 1 >{ { source.data() } };
        auto const sourceLengths =
            std::array< GLint, 1 >{ { static_cast< GLint >( source.size() ) } };
        glShaderSource( shaderObject, 1, sources.data(), sourceLengths.data() );
    }

    void compileShader( GLuint shaderObject )
    {
        glCompileShader( shaderObject );

        // check error log for info
        GLint compileStatus = GL_FALSE;
        glGetShaderiv( shaderObject, GL_COMPILE_STATUS, &compileStatus );

        if( compileStatus != GL_TRUE )
        {
            throw std::runtime_error( getInfoLog( shaderObject ) );
        }
    }

    auto getInfoLog( GLuint shaderObject ) -> std::string
    {
        auto buffer = std::array< GLchar, 256 >{};
        auto strLength = GLsizei{ 0 };
        glGetShaderInfoLog( shaderObject, buffer.size(), &strLength, buffer.data() );

        if( strLength <= 0 )
        {
            return "No error message";
        }

        return "Shader error: " + std::string( buffer.data() );
    }
}
