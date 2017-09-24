#pragma once

#include <ttb/utils/dataIO/SlotType.hpp>

#include <memory>


namespace ttb
{
    template < SlotType TType, typename TData >
    class Receiver;
}


namespace ttb
{
    template < SlotType TType, typename TData >
    class Provider;


    template < typename TData >
    class Provider< SlotType::ACTIVE, TData >
    {
    public:
        virtual ~Provider();

        virtual void
            receiver( std::shared_ptr< Receiver< SlotType::PASSIVE, TData > > receiver ) = 0;
    };


    template < typename TData >
    class Provider< SlotType::PASSIVE, TData >
    {
    public:
        virtual ~Provider();

        virtual TData pull() = 0;
    };
}


namespace ttb
{
    template < typename TData >
    Provider< SlotType::ACTIVE, TData >::~Provider() = default;


    template < typename TData >
    Provider< SlotType::PASSIVE, TData >::~Provider() = default;
}
