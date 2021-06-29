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

        /// @copydoc WrappedElement::fit( Size const& )
        virtual Size fit( Size const& size ) override;

        virtual void render( ttb::State& state ) const override;

        virtual std::string info() const override;

    private:
        Position transform( Position const& pos ) const;

        Position transformInv( Position const& pos ) const;

        HAlignment m_hAlign;
        VAlignment m_vAlign;

        Position m_offset{ 0.0f, 0.0f };
    };
}
