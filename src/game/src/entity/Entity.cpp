#include <ttb/entity/Entity.hpp>


namespace ttb
{
    Entity::~Entity() = default;

    void Entity::addComponent( Component* component )
    {
        m_components.push_back( component );
    }
}
