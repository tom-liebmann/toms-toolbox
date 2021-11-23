#pragma once

#include <ttb/core/Viewport.hpp>
#include <ttb/core/uniform/UniformHolder.hpp>
#include <ttb/math/Matrix.hpp>

#include <memory>
#include <optional>
#include <stack>
#include <unordered_map>


// forward declarations
//=============================================================================

namespace ttb
{
    class Program;
    class ViewportBinder;
    class RenderTarget;
    class Geometry;
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
            RenderTarget const* renderTarget{ nullptr };

            std::unordered_map< std::string, std::unique_ptr< UniformHolder > > uniforms;

            std::optional< Viewport > viewport;
            bool enabledScissors{ false };

            std::stack< GLint > arrayObjectStack;

            std::stack< GLint > frameBufferObjectStack;

            void applyViewport() const;
        };

        State();

        ~State();

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

        void draw( Geometry const& geometry );

        Program const& program();

        RenderTarget const& renderTarget() const;

        template < typename TType >
        TType const* uniform( std::string const& name ) const;

    private:
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
        return &dynamic_cast< TypedUniformHolder< TType > const& >( *iter->second ).value();
#else
        return &static_cast< TypedUniformHolder< TType > const& >( *iter->second ).value();
#endif
    }
}
