#include <ttb/ui/utils/DoubleQuad.hpp>

#include <ttb/ui/Framework.hpp>


namespace ttb::ui
{
    DoubleQuad::DoubleQuad( Framework& framework,
                            ColorRgb const& outerColor,
                            float outerRadius,
                            ColorRgb const& innerColor,
                            float innerRadius,
                            float thickness )
        : Element{ framework }
        , m_thickness{ thickness }
        , m_quadRenderer{ framework.resourceManager() }
    {
        m_quadRenderer.add()
            .radius( outerRadius )
            .color( { outerColor.rF(), outerColor.gF(), outerColor.bF() } );

        m_quadRenderer.add()
            .radius( innerRadius )
            .color( { innerColor.rF(), innerColor.gF(), innerColor.bF() } );
    }

    void DoubleQuad::outerColor( ColorRgb const& color )
    {
        m_quadRenderer.get( 0 ).color( { color.rF(), color.gF(), color.bF() } );
    }

    void DoubleQuad::innerColor( ColorRgb const& color )
    {
        m_quadRenderer.get( 1 ).color( { color.rF(), color.gF(), color.bF() } );
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
