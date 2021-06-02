#include <ttb/ui/utils/SingleQuad.hpp>

#include <ttb/ui/Framework.hpp>


namespace ttb::ui
{
    void SingleQuad::color( ColorRgb const& value )
    {
        m_quadRenderer.get( 0 ).color( { value.rF(), value.gF(), value.bF() } );
    }

    void SingleQuad::range( Range const& range )
    {
        Element::range( range );

        m_quadRenderer.get( 0 )
            .min( range.min( 0 ), range.min( 1 ) )
            .max( range.max( 0 ), range.max( 1 ) );
    }

    void SingleQuad::render( ttb::State& state ) const
    {
        m_quadRenderer.flush();
        m_quadRenderer.draw( state );
    }

    SingleQuad::SingleQuad( Framework& framework, ColorRgb const& color, float radius )
        : Element{ framework }, m_quadRenderer{ framework.resourceManager() }
    {
        m_quadRenderer.add().radius( radius ).color( { color.rF(), color.gF(), color.bF() } );
    }
}
