#pragma once

#include <ttb/core/gl.hpp>

#include <ttb/math/Matrix.hpp>
#include <ttb/math/Vector.hpp>

#include <initializer_list>
#include <memory>
#include <vector>

// forward declarations
//=============================================================================

namespace ttb
{
    class ProgramCreator;
    class Shader;
    class VertexAttributeList;
    class UniformStackBase;
}



// declarations
//=============================================================================

namespace ttb
{
    class Program
    {
        class Creator;

    public:
        static Creator create();

        ~Program();

        void use() const;
        void unuse() const;

        GLint attributeLocation( std::string const& name ) const;

        void applyUniform( std::string const& name, UniformStackBase const& uniform ) const;

    private:
        Program( std::vector< std::unique_ptr< Shader > > const& shaders );

        GLuint m_programObject;
    };



    class Program::Creator
    {
    public:
        Creator& attachShader( std::unique_ptr< Shader > shader );

        std::unique_ptr< Program > finish();

    private:
        Creator();

        std::vector< std::unique_ptr< Shader > > m_shaders;

        friend class Program;
    };
}
