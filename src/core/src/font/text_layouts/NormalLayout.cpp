#include <ttb/core/fonts/text_layouts/NormalLayout.hpp>

#include <ttb/core/fonts/Font.hpp>


namespace ttb::font
{
    void NormalLayout::computeLayout( Font const& font,
                                      float size,
                                      std::string_view text,
                                      CharacterCallback const& callback ) const
    {
        auto const scaleFactor = size / font.getEmSize();
        auto x = 0.0f;
        auto y = 0.0f;
        for( auto const character : text )
        {
            if( character == '\n' )
            {
                x = 0.0f;
                y += font.getLineHeight() * scaleFactor;
            }
            else
            {
                auto const& glyph = font.getGlyph( character );

                callback( glyph, x, y );

                x += glyph.getAdvance() * scaleFactor;
            }
        }
    }
}
