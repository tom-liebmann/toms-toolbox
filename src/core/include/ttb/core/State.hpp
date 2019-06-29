#pragma once

#include "uniform.hpp"
#include <ttb/core/Viewport.hpp>
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

    class UniformBase;

    template < typename TType >
    class Uniform;
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

        // uniforms
        template < typename TType >
        void pushUniform( std::string const& name, TType&& value );

        void popUniform( std::string const& name );

        template < typename TType >
        TType const& uniform( std::string const& name ) const;

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
        std::unordered_map< std::string, std::stack< std::unique_ptr< UniformBase > > > m_uniforms;
    };
}


namespace ttb
{
    template < typename TType >
    void State::pushUniform( std::string const& name, TType&& value )
    {
        auto iter = m_uniforms.find( name );

        if( iter == std::end( m_uniforms ) )
        {
            m_uniforms[ name ].push(
                std::make_unique< Uniform< std::decay_t< TType > > >( value ) );
        }
        else
        {
            iter->second.push( std::make_unique< Uniform< std::decay_t< TType > > >( value ) );
        }
    }

    template < typename TType >
    TType const& State::uniform( std::string const& name ) const
    {
        auto iter = m_uniforms.find( name );

        if( iter == std::end( m_uniforms ) )
        {
            throw std::runtime_error( "Accessing unknown uniform" );
        }
        else
        {
            auto const& u = dynamic_cast< Uniform< TType > const& >( *iter->second.top() );

            return u.value();
        }
    }
}
