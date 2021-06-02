#include <ttb/ui/elements/core/Rectangle.hpp>

#include <ttb/core/geometry.hpp>
#include <ttb/core/resources/Manager.hpp>
#include <ttb/core/shader.hpp>
#include <ttb/ui/Framework.hpp>


namespace ttb::ui
{
    void Rectangle::render( ttb::State& state ) const
    {
        auto const innerRange = Range{
            { 0.0f, 0.0f },
            { 1.0f, 1.0f },
        };

        auto const transform = ttb::mat::transform( innerRange, range() );

        auto const uniformHandle1 =
            state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ).push( transform );
        auto const uniformHandle2 = state.uniform< ttb::Vector< float, 3 > >( "u_color" )
                                        .push( { m_color.rF(), m_color.gF(), m_color.bF() } );

        state.with( *m_program, [ & ] {
            m_geometry->draw( state );  //
        } );
    }

    Rectangle::Rectangle( Framework& framework, ColorRgb const& color )
        : Element{ framework }, m_color( color )
    {
        m_program = framework.resourceManager().get< ttb::Program >( "ui_rect" );

        auto vertexBuffer = ttb::VertexBuffer::create( [ & ]( auto& c ) {
            c.attribute( GL_FLOAT, 2 );  //
        } );

        vertexBuffer->push_back().set( 0, 0.0f, 0.0f );
        vertexBuffer->push_back().set( 0, 0.0f, 1.0f );
        vertexBuffer->push_back().set( 0, 1.0f, 0.0f );
        vertexBuffer->push_back().set( 0, 1.0f, 1.0f );

        m_geometry = ttb::Geometry::create( GL_TRIANGLE_STRIP )
                         .attribute( "in_vertex", std::move( vertexBuffer ) )
                         .finish();
    }
}
