#include <ttb/entity/Entity.hpp>

#include <ttb/entity/Component.hpp>


namespace ttb
{
    Entity::Entity() = default;

    Entity::~Entity() = default;

    void Entity::init()
    {
        for( auto const comp : m_components )
        {
            comp->init();
        }
    }

    void Entity::destroy()
    {
        for( auto const comp : m_components )
        {
            comp->destroy();
        }
    }

    void Entity::addComponent( Component& component )
    {
        m_components.push_back( &component );
    }
}
