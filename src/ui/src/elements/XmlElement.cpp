#include <ttb/ui/elements/XmlElement.hpp>

#include <ttb/ui/XmlLoader.hpp>


namespace
{
    std::unique_ptr< rapidxml::xml_node<> > loadSource( char const* source );
}


namespace ttb::ui
{
    XmlElement::XmlElement( Framework& framework, rapidxml::xml_node<> const& node )
        : WrappedElement{ framework }
    {
        auto firstChild = &node;

        while( firstChild && firstChild->type() != 1 )
        {
            firstChild = firstChild->first_node();
        }

        if( !firstChild )
        {
            std::cout << "No node found" << std::endl;
            return;
        }

        auto loader = XmlLoader{ *this };

        wrappedChild( loader.loadElement( framework, *firstChild ) );
    }

    XmlElement::XmlElement( Framework& framework, char const* source )
        : XmlElement{ framework, *loadSource( source ) }
    {
    }

    Element* XmlElement::getElementById( std::string const& id )
    {
        auto const iter = m_elementIdIndex.find( id );

        return iter == std::end( m_elementIdIndex ) ? nullptr : iter->second;
    }
}


namespace
{
    std::unique_ptr< rapidxml::xml_node<> > loadSource( char const* source )
    {
        auto doc = std::make_unique< rapidxml::xml_document<> >();
        doc->parse< rapidxml::parse_non_destructive >( const_cast< char* >( source ) );
        return doc;
    }
}
