#include <ttb/ui/elements/Rectangle.hpp>

#include <ttb/core/geometry.hpp>
#include <ttb/core/resources/Manager.hpp>
#include <ttb/core/shader.hpp>
#include <ttb/core/uniform.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/ui/Framework.hpp>
#include <ttb/ui/XmlFactory.hpp>


namespace
{
    auto const factory = ttb::ui::XmlFactory< ttb::ui::Rectangle >{ "rectangle" };
}


namespace ttb::ui
{
    Rectangle::Rectangle( Framework& framework, ColorRgb const& color )
        : Element{ framework }, m_color( color )
    {
        m_program = framework.resourceManager().get< ttb::Program >( "ui_rect" );

        auto vertexBuffer = ttb::VertexBuffer::create(
            [ & ]( auto& c )
            {
                c.attribute( GL_FLOAT, 2 );  //
            } );

        vertexBuffer->push_back().set( 0, 0.0f, 0.0f );
        vertexBuffer->push_back().set( 0, 0.0f, 1.0f );
        vertexBuffer->push_back().set( 0, 1.0f, 0.0f );
        vertexBuffer->push_back().set( 0, 1.0f, 1.0f );
        vertexBuffer->flush();

        m_geometry = ttb::Geometry::create( GL_TRIANGLE_STRIP )
                         .attribute( "in_vertex", std::move( vertexBuffer ) )
                         .finish();
    }

    Rectangle::Rectangle( Framework& framework,
                          rapidxml::xml_node<> const& node,
                          XmlLoader& loader )
        : Element{ framework }
    {
    }

    Rectangle::~Rectangle() = default;

    void Rectangle::color( ColorRgb const& value )
    {
        m_color = value;
    }

    void Rectangle::render( ttb::State& state ) const
    {
        state.with(
            *m_program,
            ttb::UniformBinder{ "u_transform", ttb::mat::scale( size() ) },
            ttb::UniformBinder{
                "u_color", ttb::Vector< float, 3 >{ m_color.rF(), m_color.gF(), m_color.bF() } },
            [ & ]
            {
                state.draw( *m_geometry );
            } );
    }
}
