#pragma once

#include <memory>


namespace ttb
{
    template < typename TType >
    class UniqueCreate
    {
    public:
        template < typename... TArgs >
        static std::unique_ptr< TType > create( TArgs&&... );
    };


    template < typename TType >
    template < typename... TArgs >
    std::unique_ptr< TType > UniqueCreate< TType >::create( TArgs&&... args )
    {
        return std::unique_ptr< TType >{ new TType{ std::forward< TArgs >( args )... } };
    }
}
