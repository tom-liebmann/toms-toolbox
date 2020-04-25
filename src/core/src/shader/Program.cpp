#include <ttb/core/shader/Program.hpp>

#include <ttb/core/shader/Shader.hpp>
#include <ttb/core/uniform/stacks/UniformStack.hpp>
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
            auto const errorMsg = [&]() -> std::string {
                std::array< char, 256 > buffer;
                GLsizei strLength = 0;
                glGetProgramInfoLog( m_programObject, buffer.size(), &strLength, buffer.data() );

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

    Program::~Program()
    {
        glDeleteProgram( m_programObject );
    }

    void Program::use() const
    {
        glUseProgram( m_programObject );
    }

    void Program::unuse() const
    {
        glUseProgram( 0 );
    }

    GLint Program::attributeLocation( std::string const& name ) const
    {
        return glGetAttribLocation( m_programObject,
                                    reinterpret_cast< GLchar const* >( name.c_str() ) );
    }

    void Program::applyUniform( std::string const& name, UniformStackBase const& uniform ) const
    {
        auto location = glGetUniformLocation( m_programObject,
                                              reinterpret_cast< const GLchar* >( name.c_str() ) );

        if( location != -1 )
        {
            uniform.apply( location );
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
        return std::unique_ptr< Program >( new Program( std::move( m_shaders ) ) );
    }
}
