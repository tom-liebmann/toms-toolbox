#include <ttb/core/fonts/text_layouts/NormalLayout.hpp>

#include <ttb/core/fonts/Font.hpp>


namespace ttb::font
{
    void NormalLayout::computeLayout( Font const& font,
                                      float size,
                                      std::string_view text,
                                      float maxWidth,
                                      CharacterCallback const& callback )
    {
        for( auto const character : text )
        {
            auto const& glyph = font.getGlyph( character );

            callback( glyph, 0.0f, 0.0f );
        }
    }
}
