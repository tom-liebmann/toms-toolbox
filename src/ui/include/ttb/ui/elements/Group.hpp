#pragma once

#include <ttb/ui/CombinedElement.hpp>
#include <ttb/ui/XmlLoader.hpp>


namespace ttb::ui
{
    class Group : public CombinedElement
    {
    public:
        Group( Framework& framework );

        Group( Framework& framework, rapidxml::xml_node<> const& node, XmlLoader& loader );

        void addChild( Element* child, bool considerSize = true );
    };
}
