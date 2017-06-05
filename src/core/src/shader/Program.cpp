#include <ttb/core/shader/Program.hpp>

#include <ttb/core/shader/Shader.hpp>
#include <ttb/math/Matrix.hpp>

#include <iostream>

namespace ttb
{
    Program::Creator Program::create()
    {
        return {};
    }

    Program::Program( std::vector< std::unique_ptr< Shader > > const& shaders )
        : m_programObject( glCreateProgram() )
    {
        for( auto& shader : shaders )
            glAttachShader( m_programObject, shader->shaderObject() );

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

    ttb::Program::~Program()
    {
        glDeleteProgram( m_programObject );
    }

    void ttb::Program::use() const
    {
        glUseProgram( m_programObject );
    }

    void ttb::Program::unuse() const
    {
        glUseProgram( 0 );
    }

    GLint ttb::Program::attributeLocation( std::string const& name ) const
    {
        return glGetAttribLocation( m_programObject,
                                    reinterpret_cast< GLchar const* >( name.c_str() ) );
    }

    GLint ttb::Program::getUniformLocation( const std::string& name ) const
    {
        return glGetUniformLocation( m_programObject,
                                     reinterpret_cast< const GLchar* >( name.c_str() ) );
    }

    void ttb::Program::setUniformInt( GLint location, GLint value )
    {
        glUniform1i( location, value );
    }

    void ttb::Program::setUniform( GLint location, GLfloat value )
    {
        glUniform1f( location, value );
    }

    void ttb::Program::setUniform( GLint location, const ttb::Matrix4f& matrix )
    {
        glUniformMatrix4fv( location, 1, GL_TRUE, matrix.ptr() );
    }

    void ttb::Program::setUniform( GLint location, const ttb::Vector< float, 3 >& vector )
    {
        glUniform3f( location, vector.x(), vector.y(), vector.z() );
    }



    Program::Creator::Creator()
    {
    }

    Program::Creator& Program::Creator::attachShader( std::unique_ptr< Shader > shader )
    {
        m_shaders.push_back( std::move( shader ) );
        return *this;
    }

    std::unique_ptr< Program > Program::Creator::finish()
    {
        return std::unique_ptr< Program >( new Program( std::move( m_shaders ) ) );
    }
}
