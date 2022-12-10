#pragma once

#include <ttb/ui/WrappedElement.hpp>
#include <ttb/ui/elements/Flex.hpp>


namespace ttb::ui
{
    class Height : public WrappedElement
    {
    public:
        Height( Root& root, float height );

        Height( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader );

        void child( Element* element );

    private:
        Flex m_flex;
    };
}
