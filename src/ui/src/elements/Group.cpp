#include <ttb/ui/elements/Group.hpp>

#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>
#include <ttb/ui/XmlFactory.hpp>


namespace ttb::ui
{
    namespace
    {
        auto const factory = XmlFactory< Group >{ "group" };
    }
}


namespace ttb::ui
{
    Group::Group( Framework& framework ) : CombinedElement{ framework }
    {
    }

    Group::Group( Framework& framework, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : CombinedElement{ framework }
    {
        for( auto child = node.first_node(); child; child = child->next_sibling() )
        {
            auto element = loader.loadElement( framework, *child );

            auto considerSize = true;

            if( auto const value = loader.attrValue( *child, "consider_size" ); value )
            {
                considerSize = ( value == "true" );
            }

            CombinedElement::add( element, considerSize );
        }
    }

    void Group::addChild( Element* child, bool considerSize )
    {
        CombinedElement::add( child, considerSize );
    }
}
