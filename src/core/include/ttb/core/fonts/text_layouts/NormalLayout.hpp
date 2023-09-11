#pragma once

#include <ttb/core/fonts/TextLayout.hpp>


namespace ttb::font
{
    class NormalLayout : public TextLayout
    {
    public:
        //! \copydoc TextLayout::computeLayout( Font const&, float, std::string_view,
        //! CharacterCallback const& ) const
        virtual void computeLayout( Font const& font,
                                    float size,
                                    std::string_view text,
                                    CharacterCallback const& callback ) const override;
    };
}
