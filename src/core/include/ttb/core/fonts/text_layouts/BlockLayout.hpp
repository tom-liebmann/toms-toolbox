#pragma once

#include <ttb/core/fonts/TextLayout.hpp>


namespace ttb::font
{
    class BlockLayout : public TextLayout
    {
    public:
        BlockLayout( float minSpaceWidth, float maxWidth );

        void setMaxWidth( float value );

        //! \copydoc TextLayout::computeLayout( Font const&, float, std::string_view, float,
        //! CharacterCallback const& ) const
        virtual void computeLayout( Font const& font,
                                    float size,
                                    std::string_view text,
                                    CharacterCallback const& callback ) const override;

    private:
        static bool isWhiteSpace( char character );

        static float computeWordWidth( Font const& font,
                                       std::string_view::const_iterator start,
                                       std::string_view::const_iterator end );

        static void printLine( Font const& font,
                               float size,
                               std::string_view::const_iterator start,
                               std::string_view::const_iterator end,
                               float spaceWidth,
                               float y,
                               CharacterCallback const& callback );

        float m_minSpaceWidth;
        float m_maxWidth;
    };
}
