#pragma once

#include <ttb/ui/XmlLoader.hpp>
#include <ttb/ui/elements/Slot.hpp>


namespace ttb::ui
{
    class Align : public Slot
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

        Align( Root& root );

        Align( Root& root, HAlignment hAlign, VAlignment vAlign );

        virtual void parseXml( XmlNode const& node, XmlLoader& loader );

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
