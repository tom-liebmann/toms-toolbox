#include "ttb/ui/XmlLoader.hpp"
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

    constexpr auto SOURCE_FRAG = R"(
        #version 100

        precision mediump float;

        uniform vec4 u_color;

        void main()
        {
            gl_FragColor = u_color;
        }
    )";

    constexpr auto SOURCE_VERT = R"(
        #version 100

        precision mediump float;

        uniform mat3 u_transform;

        attribute vec2 in_vertex;

        void main()
        {
            gl_Position = vec4( u_transform * vec3( in_vertex.xy, 1.0 ), 1.0 );
        }
    )";
}


namespace ttb::ui
{
    Rectangle::Rectangle( Root& root ) : Rectangle{ root, { use_float, 1.0f, 1.0f, 1.0f }, 1.0f }
    {
    }

    Rectangle::Rectangle( Root& root, ColorRgb const& color, float opacity )
        : Element{ root }, m_color( color ), m_opacity{ opacity }
    {
        initGeometry();
    }

    void Rectangle::parseXml( XmlNode const& node, XmlLoader& loader )
    {
        Element::parseXml( node, loader );

        if( auto const value = loader.attrValue( node, "color" ) )
        {
            m_color = ColorRgb::createHexStr( value.value() ).value();
        }

        if( auto const value = loader.getAttr< float >( node, "opacity" ) )
        {
            m_opacity = value.value();
        }

        initGeometry();
    }

    Rectangle::~Rectangle() = default;

    void Rectangle::setColor( ColorRgb const& value )
    {
        m_color = value;
    }

    void Rectangle::setOpacity( float value )
    {
        m_opacity = value;
    }

    ColorRgb const& Rectangle::getColor() const
    {
        return m_color;
    }

    float Rectangle::getOpacity() const
    {
        return m_opacity;
    }

    void Rectangle::render( ttb::State& state ) const
    {
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        state.with( *m_program,
                    ttb::UniformBinder{ "u_transform", getTransform() },
                    ttb::UniformBinder{ "u_color",
                                        ttb::Vector< float, 4 >{
                                            m_color.rF(), m_color.gF(), m_color.bF(), m_opacity } },
                    [ & ]
                    {
                        state.draw( *m_geometry );
                    } );
    }

    void Rectangle::initGeometry()
    {
        m_program = ttb::Program::create(
            []( auto& c )
            {
                c.attachShader(
                    ttb::Shader::fromSource( ttb::Shader::Type::FRAGMENT, SOURCE_FRAG ) );
                c.attachShader( ttb::Shader::fromSource( ttb::Shader::Type::VERTEX, SOURCE_VERT ) );
            } );

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
