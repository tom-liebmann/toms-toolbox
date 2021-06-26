#pragma once

#include <ttb/ui/WrappedElement.hpp>


namespace ttb::ui
{
    class Center : public WrappedElement
    {
    public:
        enum class HAlignment
        {
            LEFT,
            CENTER,
            RIGHT,
        };

        enum class VAlignment
        {
            TOP,
            MIDDLE,
            BOTTOM,
        };

        Center( Framework& framework, HAlignment hAlign, VAlignment vAlign );

        void child( Element* element );

        /// @copydoc WrappedElement::range( Range const& )
        virtual void range( Range const& range ) override;

        /// @copydoc WrappedElement::fit( Range const& )
        virtual Range fit( Range const& space ) override;

    private:
        HAlignment m_hAlign;
        VAlignment m_vAlign;
    };
}
