#pragma once

#include <ttb/ui/elements/Slot.hpp>

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
    class XmlElement : public Slot
    {
    public:
        XmlElement( Root& root, XmlNode const& node );

        XmlElement( Root& root, char const* source );

        Element* getElementById( std::string const& id );

        template < typename TType >
        TType* getTypeById( std::string const& id );

        template < typename TType >
        TType& getById( std::string const& id );

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

    template < typename TType >
    inline TType& XmlElement::getById( std::string const& id )
    {
        auto const result = getTypeById< TType >( id );
        if( !result )
        {
            throw std::runtime_error( "Unable to find element with id " + id );
        }
        return *result;
    }
}
