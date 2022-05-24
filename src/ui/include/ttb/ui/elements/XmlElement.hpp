#pragma once

#include <ttb/ui/WrappedElement.hpp>

#include <rapidxml/rapidxml.hpp>

#include <map>
#include <memory>
#include <vector>


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

        template < typename TType >
        TType* getTypeById( std::string const& id );

#ifndef TEST
    private:
#endif
        std::vector< std::unique_ptr< Element > > m_elements;
        std::unordered_map< std::string, Element* > m_elementIdIndex;

        friend XmlLoader;
    };
}


namespace ttb::ui
{
    template < typename TType >
    inline TType* XmlElement::getTypeById( std::string const& id )
    {
        return dynamic_cast< TType* >( getElementById( id ) );
    }
}
