#include <ttb/core/shader/Program.hpp>

#include <ttb/core/shader/Shader.hpp>
#include <ttb/math/Matrix.hpp>

#include <iostream>

namespace ttb
{
    Program::~Program()
    {
        glDeleteProgram( m_object );
    }

    GLint Program::attributeLocation( std::string const& name ) const
    {
        return glGetAttribLocation( m_object, reinterpret_cast< GLchar const* >( name.c_str() ) );
    }

    GLint Program::uniformLocation( std::string const& name ) const
    {
        return glGetUniformLocation( m_object, reinterpret_cast< GLchar const* >( name.c_str() ) );
    }

    Program::Program( std::vector< std::unique_ptr< Shader > > const& shaders )
        : m_object{ glCreateProgram() }
    {
        for( auto& shader : shaders )
        {
            glAttachShader( m_object, shader->shaderObject() );
        }

        glLinkProgram( m_object );

        // check error log for info
        GLint linkStatus;
        glGetProgramiv( m_object, GL_LINK_STATUS, &linkStatus );

        if( !linkStatus )
        {
            auto const errorMsg = [ & ]() -> std::string
            {
                std::array< char, 256 > buffer;
                GLsizei strLength = 0;
                glGetProgramInfoLog( m_object, buffer.size(), &strLength, buffer.data() );

                if( strLength > 0 )
                {
                    return "Program error: " + std::string( buffer.data() );
                }
                else
                {
                    return "No error message";
                }
            }();

            throw std::runtime_error( errorMsg );
        }
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
        return std::unique_ptr< Program >{ new Program{ std::move( m_shaders ) } };
    }


    Program::Binder::~Binder()
    {
        m_data.program = m_oldProgram;

        if( m_oldProgram )
        {
            glUseProgram( m_oldProgram->m_object );
        }
        else
        {
            glUseProgram( 0 );
        }
    }

    Program::Binder::Binder( Program const& obj, State::Data& data )
        : m_oldProgram{ data.program }, m_data{ data }
    {
        m_data.program = &obj;
        glUseProgram( obj.m_object );
    }
}
