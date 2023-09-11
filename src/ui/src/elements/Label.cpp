#include <ttb/ui/elements/Label.hpp>

#include <ttb/core/fonts/TextFactory.hpp>
#include <ttb/core/fonts/text_layouts/NormalLayout.hpp>
#include <ttb/core/resources/Manager.hpp>
#include <ttb/core/uniform.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>
#include <ttb/ui/Root.hpp>

#include <ttb/ui/XmlFactory.hpp>

#include <fmt/core.h>

#include <regex>


namespace
{
    auto const factory = ttb::ui::XmlFactory< ttb::ui::Label >{ "label" };
}


namespace ttb::ui
{
    Label::Label( Root& root, std::string text, float size, ColorRgb const& color )
        : Element{ root }, m_text( std::move( text ) ), m_size( size ), m_color( color )
    {
        auto& resMngr = root.getResourceManager();

        m_shader = resMngr.get< ttb::Program >( "ui_label" );
        m_font = resMngr.get< ttb::Font >( "simple" );
        m_texture = resMngr.get< ttb::Texture2D >( "font_simple" );

        updateGeometry();
    }

    Label::Label( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : Element{ root }, m_size{ 1.0f }, m_color{ use_uint8, 0, 0, 0 }
    {
        auto& resMngr = root.getResourceManager();

        if( auto const value = loader.getAttr< float >( node, "size" ) )
        {
            m_size = value.value();
        }

        if( auto const value = loader.attrValue( node, "color" ) )
        {
            m_color = ColorRgb::createHexStr( value.value() ).value();
        }

        if( auto child = node.first_node() )
        {
            m_text = std::string( child->value(), child->value_size() );
        }

        m_shader = resMngr.get< ttb::Program >( "ui_label" );
        m_font = resMngr.get< ttb::Font >( "simple" );
        m_texture = resMngr.get< ttb::Texture2D >( "font_simple" );

        updateGeometry();
    }

    void Label::text( std::string value )
    {
        m_text = std::move( value );

        updateGeometry();

        changed();
    }

    void Label::setSize( float value )
    {
        m_size = value;

        updateGeometry();

        changed();
    }

    void Label::setColor( ColorRgb const& color )
    {
        m_color = color;

        updateGeometry();
    }

    auto Label::fit( Size const& /* size */ ) -> Size
    {
        return m_fontRange.extent();
    }

    void Label::render( ttb::State& state ) const
    {
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        // clang-format off
        auto const fontTransform = ttb::Matrix< float, 3, 3 >{
            1.0f / m_fontRange.extent( 0 ),                         0.0f, -m_fontRange.getMin( 0 ) / m_fontRange.extent( 0 ),
                                    0.0f, 1.0f / m_fontRange.extent( 1 ), -m_fontRange.getMin( 1 ) / m_fontRange.extent( 1 ),
                                    0.0f,                           0.0f, 1.0f,
        };
        // clang-format on

        state.with(
            *m_shader,
            ttb::UniformBinder{ "u_texture", 0 },
            ttb::UniformBinder{
                "u_color", ttb::Vector< float, 3 >{ m_color.rF(), m_color.gF(), m_color.bF() } },
            ttb::UniformBinder{ "u_transform", transform() * fontTransform },
            [ & ]
            {
                m_texture->bind( 0 );
                {
                    state.draw( *m_textGeometry );
                }
                m_texture->unbind( 0 );
            } );

        glDisable( GL_BLEND );
    }

    void Label::updateGeometry()
    {
        m_textGeometry =
            ttb::TextFactory::createText( *m_font, m_size, m_text, font::NormalLayout{} );

        m_fontRange =
            ttb::TextFactory::getDimensions( *m_font, m_size, m_text, font::NormalLayout{} );
    }
}
