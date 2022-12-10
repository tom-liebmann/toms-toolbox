#pragma once

#include <ttb/ui/Element.hpp>
#include <ttb/ui/fromStringView.hpp>

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

        template < typename TType >
        static std::optional< TType > getAttr( rapidxml::xml_node<> const& node,
                                               std::string_view const& name );

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


namespace ttb::ui
{
    template < typename TType >
    std::optional< TType > XmlLoader::getAttr( rapidxml::xml_node<> const& node,
                                               std::string_view const& name )
    {
        for( auto attr = node.first_attribute(); attr; attr = attr->next_attribute() )
        {
            auto const attrName = std::string_view{ attr->name(), attr->name_size() };
            if( name == attrName )
            {
                return fromStringView< TType >::convert(
                    std::string_view{ attr->value(), attr->value_size() } );
            }
        }

        return {};
    }
}
