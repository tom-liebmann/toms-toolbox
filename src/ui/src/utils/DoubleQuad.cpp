#include <ttb/ui/utils/DoubleQuad.hpp>

#include <ttb/ui/Root.hpp>


namespace ttb::ui
{
    DoubleQuad::DoubleQuad( Root& root,
                            ColorRgb const& outerColor,
                            float outerRadius,
                            ColorRgb const& innerColor,
                            float innerRadius,
                            float thickness )
        : Element{ root }, m_thickness{ thickness }, m_quadRenderer{ root.getResourceManager() }
    {
        m_quadRenderer.add().radius( outerRadius ).color( outerColor );

        m_quadRenderer.add().radius( innerRadius ).color( innerColor );
    }

    void DoubleQuad::outerColor( ColorRgb const& color )
    {
        m_quadRenderer.get( 0 ).color( color );
    }

    void DoubleQuad::innerColor( ColorRgb const& color )
    {
        m_quadRenderer.get( 1 ).color( color );
    }

    auto DoubleQuad::fit( Size const& size ) -> Size
    {
        auto const result = Element::fit( size );

        m_quadRenderer.get( 0 ).min( 0.0f, 0.0f ).max( result( 0 ), result( 1 ) );

        m_quadRenderer.get( 1 )
            .min( m_thickness, m_thickness )
            .max( result( 0 ) - m_thickness, result( 1 ) - m_thickness );

        return result;
    }

    void DoubleQuad::render( ttb::State& state ) const
    {
        m_quadRenderer.flush();
        m_quadRenderer.draw( state );
    }
}
