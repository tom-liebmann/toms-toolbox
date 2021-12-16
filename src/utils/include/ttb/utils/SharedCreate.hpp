#pragma once

#include <memory>


namespace ttb
{
    template < typename TType >
    class SharedCreate
    {
    public:
        template < typename... TArgs >
        static std::shared_ptr< TType > create( TArgs&&... );
    };

    template < typename TType >
    class SharedCreateInit
    {
    public:
        template < typename... TArgs >
        static std::shared_ptr< TType > create( TArgs&&... );
    };

}  // namespace ttb


namespace ttb
{
    template < typename TType >
    template < typename... TArgs >
    std::shared_ptr< TType > SharedCreate< TType >::create( TArgs&&... args )
    {
        return std::shared_ptr< TType >{ new TType{ std::forward< TArgs >( args )... } };
    }



    template < typename TType >
    template < typename... TArgs >
    std::shared_ptr< TType > SharedCreateInit< TType >::create( TArgs&&... args )
    {
        auto obj = std::shared_ptr< TType >{ new TType{ std::forward< TArgs >( args )... } };
        obj->init();
        return obj;
    }

}  // namespace ttb
