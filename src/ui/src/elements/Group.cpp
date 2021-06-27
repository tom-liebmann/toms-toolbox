#include <ttb/ui/elements/Group.hpp>

#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>


namespace ttb::ui
{
    Group::Group( Framework& framework ) : CombinedElement{ framework }
    {
    }

    void Group::addChild( Element* child, bool considerSize )
    {
        CombinedElement::add( child, considerSize );
    }
}
