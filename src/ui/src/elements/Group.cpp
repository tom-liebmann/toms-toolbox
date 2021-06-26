#include <ttb/ui/elements/Group.hpp>

#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>


namespace ttb::ui
{
    void Group::addChild( Element* child, bool considerSize )
    {
        CombinedElement::add( child, considerSize );
    }
}
