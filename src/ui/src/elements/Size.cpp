#include <ttb/ui/elements/Size.hpp>

#include <ttb/ui/XmlFactory.hpp>


namespace ttb::ui
{
    namespace
    {
        auto const factory = XmlFactory< Size >{ "size" };
    }
}


namespace ttb::ui
{
    Size::Size( Root& root, float width, float height )
        : WrappedElement{ root }, m_width{ width }, m_height{ height }
    {
    }

    Size::Size( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : WrappedElement{ root }
    {
        if( auto child = node.first_node() )
        {
            wrappedChild( loader.loadElement( root, *child ) );
        }

        if( auto const value = loader.getAttr< float >( node, "width" ) )
        {
            m_width = value.value();
        }

        if( auto const value = loader.getAttr< float >( node, "height" ) )
        {
            m_height = value.value();
        }
    }

    void Size::child( Element* element )
    {
        wrappedChild( element );
    }

    auto Size::fit( Element::Size const& /* size */ ) -> Element::Size
    {
        if( auto const child = wrappedChild(); child )
        {
            child->fit( { m_width, m_height } );
        }

        return Element::fit( { m_width, m_height } );
    }
}
