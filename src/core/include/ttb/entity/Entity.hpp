#pragma once

#include <ttb/core/State.hpp>

#include <vector>

namespace ttb
{
    class Entity
    {
    public:
        Entity();

        Entity( Entity& parent );

        virtual ~Entity();

        void setParent( Entity* newParent );

        void draw( ttb::State& state ) const;

    private:
        void addChild( Entity& entity );

        void removeChild( Entity const& entity );

        Entity* m_parent{ nullptr };
        std::vector< Entity* > m_children;
    };
}
