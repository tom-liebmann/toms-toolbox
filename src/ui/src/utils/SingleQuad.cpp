#include <ttb/ui/utils/SingleQuad.hpp>

#include <ttb/core/uniform.hpp>
#include <ttb/ui/Root.hpp>


namespace ttb::ui
{
    SingleQuad::SingleQuad( Root& root, ColorRgb const& color, float radius )
        : Element{ root }, m_quadRenderer{ root.getResourceManager() }
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
