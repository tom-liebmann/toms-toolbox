#pragma once

#include <ttb/core/Bindable.hpp>
#include <ttb/core/gl.hpp>

#include "ShaderType.hpp"
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
    class Program : public Bindable< Program >
    {
    public:
        class Builder;

        ~Program();

        GLint attributeLocation( std::string const& name ) const;

        GLint uniformLocation( std::string const& name ) const;

    private:
        Program( std::vector< std::unique_ptr< Shader > > const& shaders );

        GLuint m_object;

        friend Binder;
    };



    class Program::Builder
    {
    public:
        auto addShader( std::unique_ptr< Shader > shader ) -> Builder&;

        auto addShaderSource( ShaderType type, std::string_view source ) -> Builder&;

        auto build() -> std::unique_ptr< Program >;

    private:
        std::vector< std::unique_ptr< Shader > > m_shaders;
    };



    template <>
    class Bindable< Program >::Binder
    {
    public:
        ~Binder();

    private:
        Binder( Program const& obj, State::Data& data );

        Program const* m_oldProgram;
        State::Data& m_data;

        friend Bindable;
    };
}
