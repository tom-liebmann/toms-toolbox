#pragma once

#include <ttb/core/State.hpp>

#include <vector>


namespace ttb
{
    class Component;
}

namespace ttb
{
    class Entity
    {
    public:
        Entity();

        virtual ~Entity();

        virtual void init();

        virtual void destroy();

        template < typename TType >
        auto getComponent() -> TType*;

    private:
        void addComponent( Component& component );

        std::vector< Component* > m_components;

        friend Component;
    };
}

namespace ttb
{
    template < typename TType >
    inline auto Entity::getComponent() -> TType*
    {
        for( auto const comp : m_components )
        {
            if( auto const typedComp = dynamic_cast< TType* >( comp ) )
            {
                return typedComp;
            }
        }
        return nullptr;
    }
}
