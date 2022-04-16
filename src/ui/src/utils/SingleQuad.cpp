#include <ttb/ui/utils/SingleQuad.hpp>

#include <ttb/core/uniform.hpp>
#include <ttb/ui/Framework.hpp>


namespace ttb::ui
{
    SingleQuad::SingleQuad( Framework& framework, ColorRgb const& color, float radius )
        : Element{ framework }, m_quadRenderer{ framework.resourceManager() }
    {
        m_quadRenderer.add().radius( radius ).color( color );
    }

    void SingleQuad::color( ColorRgb const& value )
    {
        m_quadRenderer.get( 0 ).color( value );
    }

    void SingleQuad::offset( Offset const& value )
    {
        m_quadRenderer.get( 0 )
            .min( value( 0 ), value( 1 ) )
            .max( value( 0 ) + size()( 0 ), value( 1 ) + size()( 1 ) );
    }

    void SingleQuad::render( ttb::State& state ) const
    {
        m_quadRenderer.flush();
        m_quadRenderer.draw( state );
    }
}
