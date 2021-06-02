#pragma once

#include <memory>


template < typename TType >
class SharedCreate
{
public:
    template < typename... TArgs >
    static std::shared_ptr< TType > create( TArgs&&... );
};


template < typename TType >
template < typename... TArgs >
std::shared_ptr< TType > SharedCreate< TType >::create( TArgs&&... args )
{
    return std::shared_ptr< TType >{ new TType{ std::forward< TArgs >( args )... } };
}
