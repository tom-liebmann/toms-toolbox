#pragma once

#include <ttb/utils/dataIO/SlotType.hpp>

#include <functional>
#include <memory>


namespace ttb
{
    template < SlotType TType, typename TData >
    class Provider;
}


namespace ttb
{
    template < SlotType TType, typename TData >
    class Receiver;


    template < typename TData >
    class Receiver< SlotType::ACTIVE, TData >
    {
    public:
        virtual ~Receiver();

        virtual void
            provider( std::shared_ptr< Provider< SlotType::PASSIVE, TData > > provider ) = 0;
    };


    template < typename TData >
    class Receiver< SlotType::PASSIVE, TData >
    {
    public:
        virtual ~Receiver();

        virtual void push( TData data ) = 0;
    };
}


namespace ttb
{
    template < typename TData >
    Receiver< SlotType::ACTIVE, TData >::~Receiver() = default;


    template < typename TData >
    Receiver< SlotType::PASSIVE, TData >::~Receiver() = default;
}
