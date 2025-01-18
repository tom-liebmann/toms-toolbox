#include <ttb/entity/Entity.hpp>

#include <algorithm>


namespace ttb
{
    Entity::Entity() = default;

    Entity::Entity( Entity& parent )
    {
        setParent( &parent );
    }

    Entity::~Entity() = default;

    void Entity::setParent( Entity* newParent )
    {
        if( m_parent )
        {
            m_parent->removeChild( *this );
        }

        m_parent = newParent;

        if( m_parent )
        {
            newParent->addChild( *this );
        }
    }

    void Entity::draw( ttb::State& state ) const
    {
        for( auto const child : m_children )
        {
            child->draw( state );
        }
    }

    void Entity::addChild( Entity& entity )
    {
        m_children.push_back( &entity );
    }

    void Entity::removeChild( Entity const& entity )
    {
        m_children.erase( std::remove( std::begin( m_children ), std::end( m_children ), &entity ),
                          std::end( m_children ) );
    }
}
