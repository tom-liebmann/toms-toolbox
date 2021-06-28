#include <ttb/ui/utils/SingleQuad.hpp>

#include <ttb/ui/Framework.hpp>


namespace ttb::ui
{
    SingleQuad::SingleQuad( Framework& framework, ColorRgb const& color, float radius )
        : Element{ framework }, m_quadRenderer{ framework.resourceManager() }
    {
        m_quadRenderer.add().radius( radius ).color( { color.rF(), color.gF(), color.bF() } );
    }

    void SingleQuad::color( ColorRgb const& value )
    {
        m_quadRenderer.get( 0 ).color( { value.rF(), value.gF(), value.bF() } );
    }

    auto SingleQuad::fit( Size const& size ) -> Size
    {
        auto const result = Element::fit( size );

        m_quadRenderer.get( 0 ).min( 0.0f, 0.0f ).max( size( 0 ), size( 1 ) );

        return result;
    }

    void SingleQuad::render( ttb::State& state ) const
    {
        m_quadRenderer.flush();
        m_quadRenderer.draw( state );
    }
}
