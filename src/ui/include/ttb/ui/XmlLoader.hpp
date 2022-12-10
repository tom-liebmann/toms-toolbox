#pragma once

#include <ttb/ui/Element.hpp>

#include <rapidxml/rapidxml.hpp>


namespace ttb::ui
{
    class XmlFactoryBase;
    class XmlElement;

    template < typename TType >
    class XmlFactory;
}


namespace ttb::ui
{
    class XmlLoader
    {
    public:
        Element* loadElement( Root& root, rapidxml::xml_node<> const& node );

        static std::optional< std::string_view > attrValue( rapidxml::xml_node<> const& node,
                                                            std::string_view const& name );

    private:
        XmlLoader( XmlElement& parent );

        static void registerFactory( std::string_view const& name, XmlFactoryBase* factory );

        XmlElement& m_parent;

        friend XmlElement;

        template < typename TType >
        friend class XmlFactory;
    };
}
