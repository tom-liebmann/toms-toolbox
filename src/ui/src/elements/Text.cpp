#include <ttb/ui/elements/Text.hpp>

#include <ttb/core/fonts/TextFactory.hpp>
#include <ttb/core/fonts/text_layouts/BlockLayout.hpp>
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
    auto const factory = ttb::ui::XmlFactory< ttb::ui::Text >{ "text" };
}


namespace ttb::ui
{
    Text::Text( Root& root ) : Text{ root, "", 0.1f, { ttb::use_float, 0.0f, 0.0f, 0.0f } }
    {
    }

    Text::Text( Root& root, std::string text, float size, ColorRgb const& color )
        : Element{ root }, m_text( std::move( text ) ), m_size( size ), m_color( color )
    {
        auto& resMngr = root.getResourceManager();

        m_shader = resMngr.get< ttb::Program >( "ui_label" );
        m_font = resMngr.get< ttb::Font >( "simple" );
        m_texture = resMngr.get< ttb::Texture2D >( "font_simple" );

        m_textLayout = std::make_unique< ttb::font::NormalLayout >();

        updateGeometry();
    }

    void Text::parseXml( XmlNode const& node, XmlLoader& loader )
    {
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

        if( auto const value = loader.getAttr< std::string >( node, "layout" ) )
        {
            if( value == "block" )
            {
                auto const spaceWidth =
                    loader.getAttr< float >( node, "spaceWidth" ).value_or( 0.1f );

                auto const maxWidth = loader.getAttr< float >( node, "maxWidth" )
                                          .value_or( std::numeric_limits< float >::infinity() );

                m_textLayout = std::make_unique< ttb::font::BlockLayout >( spaceWidth, maxWidth );
            }
        }

        updateGeometry();
    }

    void Text::setText( std::string value )
    {
        m_text = std::move( value );

        updateGeometry();

        changed();
    }

    void Text::setSize( float value )
    {
        m_size = value;

        updateGeometry();

        changed();
    }

    void Text::setColor( ColorRgb const& color )
    {
        m_color = color;

        updateGeometry();
    }

    FitExtent Text::fitWidth( Size const& space ) const
    {
        updateMaxWidth( space( 0 ) - getMargin().getRightLeft() );

        return m_fontRange.extent( 0 );
    }

    FitExtent Text::fitHeight( Size const& space ) const
    {
        updateMaxWidth( space( 0 ) - getMargin().getRightLeft() );

        return m_fontRange.extent( 1 );
    }

    void Text::setSize( Size const& value )
    {
        Element::setSize( value );

        updateMaxWidth( getSize()( 0 ) );
        m_textGeometry = ttb::TextFactory::createText( *m_font, m_size, m_text, *m_textLayout );
    }

    void Text::render( ttb::State& state ) const
    {
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        state.with(
            *m_shader,
            ttb::UniformBinder{ "u_texture", 0 },
            ttb::UniformBinder{
                "u_color", ttb::Vector< float, 3 >{ m_color.rF(), m_color.gF(), m_color.bF() } },
            ttb::UniformBinder{ "u_transform", ttb::mat::translation( getPosition() ) },
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

    void Text::updateGeometry()
    {
        m_textGeometry = ttb::TextFactory::createText( *m_font, m_size, m_text, *m_textLayout );
        m_fontRange = ttb::TextFactory::getDimensions( *m_font, m_size, m_text, *m_textLayout );
    }

    void Text::updateMaxWidth( float value ) const
    {
        if( auto blockLayout = dynamic_cast< ttb::font::BlockLayout* >( m_textLayout.get() ) )
        {
            if( value != blockLayout->getMaxWidth() )
            {
                blockLayout->setMaxWidth( value );
                m_fontRange =
                    ttb::TextFactory::getDimensions( *m_font, m_size, m_text, *m_textLayout );
            }
        }
    }
}
