#pragma once

#include <ttb/core/Bindable.hpp>
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
    class Program : public Bindable< Program >
    {
    public:
        class Creator;

        template < typename TFunc >
        static std::unique_ptr< Program > create( TFunc const& func );

        ~Program();

        GLint attributeLocation( std::string const& name ) const;

        GLint uniformLocation( std::string const& name ) const;

    private:
        Program( std::vector< std::unique_ptr< Shader > > const& shaders );

        GLuint m_object;

        friend Binder;
    };



    class Program::Creator
    {
    public:
        Creator& attachShader( std::unique_ptr< Shader > shader );

        ~Creator();

    private:
        Creator();

        std::unique_ptr< Program > finish();

        std::vector< std::unique_ptr< Shader > > m_shaders;

        friend class Program;
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


namespace ttb
{
    template < typename TFunc >
    inline std::unique_ptr< Program > Program::create( TFunc const& func )
    {
        Creator creator;
        func( creator );
        return creator.finish();
    }
}
