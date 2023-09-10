#pragma once

#include <ttb/core/fonts/TextLayout.hpp>


namespace ttb::font
{
    class NormalLayout : public TextLayout
    {
    public:
        //! \copydoc TextLayout::computeLayout( Font const&, float, std::string_view, float,
        //! CharacterCallback const& )
        virtual void computeLayout( Font const& font,
                                    float size,
                                    std::string_view text,
                                    float maxWidth,
                                    CharacterCallback const& callback ) override;
    };
}
