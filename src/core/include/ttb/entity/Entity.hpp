#pragma once

#include <ttb/core/State.hpp>
#include <ttb/utils/concepts.hpp>

#include <vector>

#include <fmt/core.h>


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

        template < ttb::Pointer TType >
        auto getComponent() -> TType;

        template < ttb::Reference TType >
        auto getComponent() -> TType;

    private:
        void addComponent( Component& component );

        std::vector< Component* > m_components;

        friend Component;
    };
}

namespace ttb
{
    template < ttb::Pointer TType >
    inline auto Entity::getComponent() -> TType
    {
        for( auto const comp : m_components )
        {
            if( auto const typedComp = dynamic_cast< TType >( comp ) )
            {
                return typedComp;
            }
        }
        return nullptr;
    }

    template < ttb::Reference TType >
    inline auto Entity::getComponent() -> TType
    {
        for( auto const comp : m_components )
        {
            if( auto const typedComp = dynamic_cast< std::remove_reference_t< TType >* >( comp ) )
            {
                return *typedComp;
            }
        }
        throw std::runtime_error( fmt::format( "Missing component: {}", typeid( TType ).name() ) );
    }
}
