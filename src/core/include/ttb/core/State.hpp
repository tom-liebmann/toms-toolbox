#pragma once

#include <ttb/core/Viewport.hpp>
#include <ttb/core/uniform/UniformHolder.hpp>
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
        struct Data
        {
            Program const* program{ nullptr };
            std::unordered_map< std::string, std::unique_ptr< UniformHolder > > uniforms;
        };

        State();
        ~State();

        // render target
        void pushTarget( RenderTarget& target );
        void popTarget();

        RenderTarget const& renderTarget() const;

        template < typename TBindable, typename TFunc >
        void with( TBindable const& bindable, TFunc const& func );

        template < typename TBindable,
                   typename TRestFirst,
                   typename TRestSecond,
                   typename... TRest >
        void with( TBindable const& bindable,
                   TRestFirst const& restFirst,
                   TRestSecond const& restSecond,
                   TRest const&... rest );

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
        TType const* uniform( std::string const& name ) const;

    private:
        // render target
        std::stack< ttb::RenderTarget* > m_renderTargetStack;

        // geometry
        GLint m_parentArrayObject;
        std::stack< GLuint > m_arrayObjectStack;

        // framebuffer
        GLint m_parentFramebufferObject;
        std::stack< GLuint > m_framebufferObjectStack;

        // viewport
        std::stack< Viewport > m_viewportStack;

        Data m_data;
    };
}


namespace ttb
{
    template < typename TBindable, typename TFunc >
    inline void State::with( TBindable const& bindable, TFunc const& func )
    {
        auto const binder = bindable.bind( m_data );
        func();
    }

    template < typename TBindable, typename TRestFirst, typename TRestSecond, typename... TRest >
    inline void State::with( TBindable const& bindable,
                             TRestFirst const& restFirst,
                             TRestSecond const& restSecond,
                             TRest const&... rest )
    {
        auto const binder = bindable.bind( m_data );
        with( restFirst, restSecond, rest... );
    }

    template < typename TType >
    TType const* State::uniform( std::string const& name ) const
    {
        auto const iter = m_data.uniforms.find( name );

        if( iter == std::end( m_data.uniforms ) )
        {
            return nullptr;
        }

#ifndef NDEBUG
        return dynamic_cast< TypedUniformHolder< TType >& >( iter->second ).value();
#else
        return static_cast< TypedUniformHolder< TType >& >( iter->second ).value();
#endif
    }
}
