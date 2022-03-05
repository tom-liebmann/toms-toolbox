#pragma once

#include <ttb/ui/WrappedElement.hpp>
#include <ttb/ui/XmlLoader.hpp>


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

        Center( Framework& framework, rapidxml::xml_node<> const& node, XmlLoader& loader );

        void child( Element* element );

        /// @copydoc WrappedElement::fit( Size const& )
        virtual Size fit( Size const& size ) override;

        virtual void render( ttb::State& state ) const override;

        virtual std::string info() const override;

        virtual bool onEvent( ttb::Event const& event ) override;

    private:
        Position transform( Position const& pos ) const;

        Position transformInv( Position const& pos ) const;

        HAlignment m_hAlign;
        VAlignment m_vAlign;

        Position m_offset{ 0.0f, 0.0f };
    };
}
