#pragma once

#include "Entity.hpp"

#include <memory>


namespace ttb
{
    template < typename TType >
    class TypedEntity : public Entity
    {
    public:
        template < typename... TArgs >
        static auto create( TArgs&&... args ) -> std::unique_ptr< TType >;
    };
}


namespace ttb
{
    template < typename TType >
    template < typename... TArgs >
    auto TypedEntity< TType >::create( TArgs&&... args ) -> std::unique_ptr< TType >
    {
        auto entity = std::unique_ptr< TType >{ new TType{ std::forward< TArgs >( args )... } };
        entity->init();
        return entity;
    }
}
