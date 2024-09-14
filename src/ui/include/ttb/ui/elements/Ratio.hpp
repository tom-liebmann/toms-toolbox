#pragma once

#include <ttb/ui/XmlLoader.hpp>
#include <ttb/ui/elements/Slot.hpp>


namespace ttb::ui
{
    class Ratio : public Slot
    {
    public:
        Ratio( Root& root, float ratio = 1.0f );

        Ratio( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader );

        virtual void parseXml( XmlNode const& node, XmlLoader& loader ) override;

        void ratio( float ratio );

        virtual FitExtent fitWidth( const Size& space ) const override;

        virtual FitExtent fitHeight( const Size& space ) const override;

        virtual void setSize( const Size& value ) override;

    private:
        float m_ratio{ 1.0f };
    };
}
