#include <ttb/ui/elements/Rectangle.hpp>

#include <ttb/core/geometry.hpp>
#include <ttb/core/resources/Manager.hpp>
#include <ttb/core/shader.hpp>
#include <ttb/core/uniform.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/ui/Root.hpp>
#include <ttb/ui/XmlFactory.hpp>


namespace
{
    auto const factory = ttb::ui::XmlFactory< ttb::ui::Rectangle >{ "rectangle" };
}


namespace ttb::ui
{
    Rectangle::Rectangle( Root& root, ColorRgb const& color ) : Element{ root }, m_color( color )
    {
        initGeometry();
    }

    Rectangle::Rectangle( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : Element{ root }
    {
        if( auto const value = loader.attrValue( node, "color" ); value )
        {
            m_color = ColorRgb::createHexStr( value.value() ).value();
        }

        initGeometry();
    }

    Rectangle::~Rectangle() = default;

    void Rectangle::color( ColorRgb const& value )
    {
        m_color = value;
    }

    ColorRgb const& Rectangle::color() const
    {
        return m_color;
    }

    void Rectangle::render( ttb::State& state ) const
    {
        state.with(
            *m_program,
            ttb::UniformBinder{ "u_transform", transform() },
            ttb::UniformBinder{
                "u_color", ttb::Vector< float, 3 >{ m_color.rF(), m_color.gF(), m_color.bF() } },
            [ & ]
            {
                state.draw( *m_geometry );
            } );
    }

    void Rectangle::initGeometry()
    {
        m_program = getRoot().getResourceManager().get< ttb::Program >( "ui_rect" );

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
}
