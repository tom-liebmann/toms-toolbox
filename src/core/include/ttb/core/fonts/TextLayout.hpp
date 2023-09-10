#pragma once

#include <functional>
#include <string_view>


namespace ttb
{
    class Font;
    class Glyph;
}


namespace ttb
{
    class TextLayout
    {
    public:
        virtual ~TextLayout();

        using CharacterCallback = std::function< void( Glyph const& glyph, float x, float y ) >;

        /**
         * Computes the position of each character with the given layout parameters.
         *
         * \param font Font that provides glyph information.
         * \param size Font size.
         * \param text Text to compute layout for.
         * \param maxWidth Maximum width of the resulting text.
         * \param callback Callable that is called with position information for every charater.
         */
        virtual void computeLayout( Font const& font,
                                    float size,
                                    std::string_view text,
                                    float maxWidth,
                                    CharacterCallback const& callback ) = 0;
    };
}
