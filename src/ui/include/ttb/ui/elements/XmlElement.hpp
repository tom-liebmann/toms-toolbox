#pragma once

#include <ttb/ui/WrappedElement.hpp>

#include <rapidxml/rapidxml.hpp>


namespace ttb::ui
{
    class XmlLoader;
}


namespace ttb::ui
{
    class XmlElement : public WrappedElement
    {
    public:
        XmlElement( Framework& framework, rapidxml::xml_node<> const& node );

    private:
        std::vector< std::unique_ptr< Element > > m_elements;

        friend XmlLoader;
    };
}
