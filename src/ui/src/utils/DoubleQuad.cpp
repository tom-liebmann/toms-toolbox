#include <ttb/ui/utils/DoubleQuad.hpp>

#include <ttb/ui/Framework.hpp>


namespace ttb::ui
{
    void DoubleQuad::outerColor( ColorRgb const& color )
    {
        m_quadRenderer.get( 0 ).color( { color.rF(), color.gF(), color.bF() } );
    }

    void DoubleQuad::innerColor( ColorRgb const& color )
    {
        m_quadRenderer.get( 1 ).color( { color.rF(), color.gF(), color.bF() } );
    }

    void DoubleQuad::range( Range const& range )
    {
        Element::range( range );

        m_quadRenderer.get( 0 )
            .min( range.min( 0 ), range.min( 1 ) )
            .max( range.max( 0 ), range.max( 1 ) );

        m_quadRenderer.get( 1 )
            .min( range.min( 0 ) + m_thickness, range.min( 1 ) + m_thickness )
            .max( range.max( 0 ) - m_thickness, range.max( 1 ) - m_thickness );
    }

    void DoubleQuad::render( ttb::State& state ) const
    {
        m_quadRenderer.flush();
        m_quadRenderer.draw( state );
    }

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
}
