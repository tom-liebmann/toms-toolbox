#include <ttb/ui/elements/Label.hpp>

#include <ttb/core/fonts/TextFactory.hpp>
#include <ttb/core/resources/Manager.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>
#include <ttb/ui/Framework.hpp>


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

    void Label::text( std::string value )
    {
        m_text = std::move( value );

        updateGeometry();

        changed();
    }

    Element::Range Label::fit( Range const& space )
    {
        auto const fontRange = m_font->textDimensions( m_size, m_text );
        return { space.min(), space.min() + fontRange.extent() };
    }

    void Label::render( ttb::State& state ) const
    {
        glEnable( GL_BLEND );

        state.with( *m_shader, [ & ] {
            auto const transform = ttb::mat::translation( Element::range().min() );

            auto const u1 =
                state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ).push( transform );

            auto const u2 = state.uniform< int >( "u_texture" ).push( 0 );

            auto const u3 = state.uniform< ttb::Vector< float, 3 > >( "u_color" )
                                .push( { m_color.rF(), m_color.gF(), m_color.bF() } );
            m_texture->bind( 0 );
            {
                m_textGeometry->draw( state );
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
