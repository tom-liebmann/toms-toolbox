#pragma once

#include <ttb/ui/WrappedElement.hpp>
#include <ttb/ui/XmlLoader.hpp>


namespace ttb::ui
{
    class Align : public WrappedElement
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

        Align( Root& root, HAlignment hAlign, VAlignment vAlign );

        Align( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader );

        void setChild( Element* element );

        virtual FitExtent fitWidth( Size const& space ) const override;

        virtual FitExtent fitHeight( Size const& space ) const override;

        virtual void setSize( Size const& value );

        virtual void setPosition( Position const& value );

    private:
        HAlignment m_hAlign;
        VAlignment m_vAlign;

        Position m_childOffset{};
    };
}
