#include <ttb/entity/Component.hpp>


namespace ttb
{
    Component::Component( Entity& entity ) : m_entity{ entity }
    {
    }

    Component::~Component() = default;
}
