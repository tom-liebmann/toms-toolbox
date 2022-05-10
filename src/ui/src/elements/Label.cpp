#include <ttb/ui/elements/Label.hpp>

#include <ttb/core/fonts/TextFactory.hpp>
#include <ttb/core/resources/Manager.hpp>
#include <ttb/core/uniform.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>
#include <ttb/ui/Framework.hpp>

#include <ttb/ui/XmlFactory.hpp>

#include <regex>


namespace ttb::ui
{
    namespace
    {
        auto const factory = XmlFactory< Label >{ "label" };
    }
}


namespace ttb::ui
{
    Label::Label( Framework& framework, std::string text, float size, ColorRgb const& color )
        : Element{ framework }, m_text( std::move( text ) ), m_size( size ), m_color( color )
    {
        auto& resMngr = framework.resourceManager();

        m_shader = resMngr.get< ttb::Program >( "ui_label" );
        m_font = resMngr.get< ttb::Font >( "simple" );
        m_texture = resMngr.get< ttb::Texture2D >( "font_simple" );

        updateGeometry();
    }

    Label::Label( Framework& framework, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : Element{ framework }, m_size{ 1.0f }, m_color{ use_uint8, 0, 0, 0 }
    {
        auto& resMngr = framework.resourceManager();

        if( auto const value = loader.attrValue( node, "size" ); value )
        {
            m_size = std::stof( std::string( *value ) );
        }

        if( auto const value = loader.attrValue( node, "color" ); value )
        {
            m_color = ColorRgb::createHexStr( value.value() ).value();
        }

        if( auto child = node.first_node(); child )
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

    auto Label::fit( Size const& /* size */ ) -> Size
    {
        auto const fontRange = m_font->textDimensions( m_size, m_text );
        return fontRange.extent();
    }

    void Label::render( ttb::State& state ) const
    {
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        auto const fontRange = m_font->textDimensions( m_size, m_text );

        state.with(
            *m_shader,
            ttb::UniformBinder{ "u_texture", 0 },
            ttb::UniformBinder{
                "u_color", ttb::Vector< float, 3 >{ m_color.rF(), m_color.gF(), m_color.bF() } },
            ttb::UniformBinder{
                "u_transform",
                transform() * ttb::mat::scale( ttb::Vector{ 1.0f / fontRange.extent( 0 ),
                                                            1.0f / fontRange.extent( 1 ) } ) },
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
        m_textGeometry = ttb::TextFactory::createText( m_size, *m_font, m_text );
    }
}
