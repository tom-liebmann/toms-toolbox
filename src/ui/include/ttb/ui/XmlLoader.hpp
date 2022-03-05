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
        Element* loadElement( Framework& framework, rapidxml::xml_node<> const& node );

    private:
        XmlLoader( XmlElement& parent );

        static void registerFactory( std::string const& name, XmlFactoryBase* factory );

        XmlElement& m_parent;

        friend XmlElement;

        template < typename TType >
        friend class XmlFactory;
    };
}
