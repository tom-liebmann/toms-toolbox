#pragma once

#include <ttb/ui/WrappedElement.hpp>

#include <rapidxml/rapidxml.hpp>

#include <map>


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

        XmlElement( Framework& framework, char const* source );

        Element* getElementById( std::string const& id );

#ifndef TEST
    private:
#endif
        std::vector< std::unique_ptr< Element > > m_elements;
        std::unordered_map< std::string, Element* > m_elementIdIndex;

        friend XmlLoader;
    };
}
