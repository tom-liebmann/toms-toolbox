#pragma once

#include "uniform.hpp"
#include <ttb/core/Viewport.hpp>
#include <ttb/core/uniform.hpp>
#include <ttb/math/Matrix.hpp>

#include <memory>
#include <stack>
#include <unordered_map>


// forward declarations
//=============================================================================

namespace ttb
{
    class Program;
    class RenderTarget;
}



// declarations
//=============================================================================

namespace ttb
{
    class State
    {
    public:
        State();
        ~State();

        // render target
        void pushTarget( std::shared_ptr< RenderTarget > const& target );
        void popTarget();

        RenderTarget const& renderTarget() const;

        // shader
        void pushProgram( std::shared_ptr< Program const > const& program );
        void popProgram();

        Program const& program();

        // geometry
        void pushArrayObject( GLuint arrayObject );
        void popArrayObject();

        // framebuffer
        void pushFramebuffer( GLuint framebufferObject );
        void popFramebuffer();

        // viewport
        void pushViewport( Viewport const& viewport );
        void popViewport();

        // events
        void apply();

        template < typename TType >
        UniformStack< TType >& uniform( std::string const& name );

        template < typename TType >
        UniformStack< TType > const& uniform( std::string const& name ) const;

    private:
        // render target
        std::stack< std::shared_ptr< ttb::RenderTarget > > m_renderTargetStack;

        // shader
        GLint m_parentProgram;
        std::stack< std::shared_ptr< Program const > > m_programStack;

        // geometry
        GLint m_parentArrayObject;
        std::stack< GLuint > m_arrayObjectStack;

        // framebuffer
        GLint m_parentFramebufferObject;
        std::stack< GLuint > m_framebufferObjectStack;

        // viewport
        std::stack< Viewport > m_viewportStack;

        // uniforms
        std::unordered_map< std::string, std::unique_ptr< UniformStackBase > > m_uniforms;
    };
}


namespace ttb
{
    template < typename TType >
    UniformStack< TType >& State::uniform( std::string const& name )
    {
        auto iter = m_uniforms.find( name );

        if( iter == std::end( m_uniforms ) )
        {
            m_uniforms[ name ] = std::make_unique< UniformStack< TType > >();
            return static_cast< UniformStack< TType >& >( *m_uniforms[ name ] );
        }
        else
        {
#ifndef NDEBUG
            return dynamic_cast< UniformStack< TType >& >( *m_uniforms[ name ] );
#else
            return static_cast< UniformStack< TType >& >( *m_uniforms[ name ] );
#endif
        }
    }

    template < typename TType >
    UniformStack< TType > const& State::uniform( std::string const& name ) const
    {
#ifndef NDEBUG
        return dynamic_cast< UniformStack< TType > const& >( m_uniforms.at( name ) );
#else
        return static_cast< UniformStack< TType > const& >( m_uniforms.at( name ) );
#endif
    }
}
