#include <ttb/entity/Component.hpp>

#include <ttb/entity/Entity.hpp>

namespace ttb
{
    Component::Component( Entity& entity ) : m_entity{ entity }
    {
        m_entity.addComponent( *this );
    }

    Component::~Component() = default;

    void Component::init()
    {
    }

    void Component::destroy()
    {
    }
}
