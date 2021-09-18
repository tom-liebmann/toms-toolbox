#include <ttb/core/State.hpp>

#include <ttb/core/RenderTarget.hpp>
#include <ttb/core/geometry/Geometry.hpp>
#include <ttb/core/shader/Program.hpp>
#include <ttb/core/window/Window.hpp>

#include <iostream>

namespace ttb
{
    State::State()
    {
    }

    State::~State()
    {
    }

    void State::draw( Geometry const& geometry )
    {
        if( m_data.program )
        {
            for( auto const& uniformPair : m_data.uniforms )
            {
                auto const location = m_data.program->uniformLocation( uniformPair.first );
                uniformPair.second->apply( location );
            }

            geometry.draw( m_data );
        }
    }

    RenderTarget const& State::renderTarget() const
    {
        assert( m_data.renderTarget != nullptr );

        return *m_data.renderTarget;
    }

    Program const& State::program()
    {
        assert( m_data.program != nullptr );

        return *m_data.program;
    }

    void State::Data::applyViewport() const
    {
        if( viewport.has_value() )
        {
            auto const& viewport = this->viewport.value();

            glViewport(
                viewport.min( 0 ), viewport.min( 1 ), viewport.max( 0 ), viewport.max( 1 ) );

            if( enabledScissors )
            {
                glScissor(
                    viewport.min( 0 ), viewport.min( 1 ), viewport.max( 0 ), viewport.max( 1 ) );

                glEnable( GL_SCISSOR_TEST );
            }
            else
            {
                glDisable( GL_SCISSOR_TEST );
            }
        }
    }
}
