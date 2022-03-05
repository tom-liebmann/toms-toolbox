#include <ttb/ui/elements/XmlElement.hpp>

#include <ttb/ui/XmlLoader.hpp>


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

    Element* XmlElement::getElementById( std::string const& id )
    {
        auto const iter = m_elementIdIndex.find( id );

        return iter == std::end( m_elementIdIndex ) ? nullptr : iter->second;
    }
}
