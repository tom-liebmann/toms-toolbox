#pragma once

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
        virtual ~Entity();

        template < typename TType >
        auto getComponent() -> TType*;

        void update( float frameTime );

        void draw( ttb::State& state ) const;

    private:
        void addComponent( Component* component );

        std::vector< Component* > m_components;

        friend class Component;
    };
}


namespace ttb
{
    template < typename TType >
    inline auto Entity::getComponent() -> TType*
    {
        for( auto const comp : m_components )
        {
            if( auto const c = dynamic_cast< TType* >( comp ) )
            {
                return c;
            }
        }

        return nullptr;
    }
}
