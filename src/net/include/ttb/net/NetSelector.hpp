#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/dataIO.hpp>

#include <queue>


namespace ttb
{
    class SelectableHolder;
}


namespace ttb
{
    class NetSelector
    {
    public:
        using EventOutput = Provider< SlotType::ACTIVE, Event& >;

        static std::unique_ptr< NetSelector > create();

        virtual ~NetSelector();

        virtual void add( std::shared_ptr< SelectableHolder > const& socket ) = 0;

        virtual void remove( std::shared_ptr< SelectableHolder > const& socket ) = 0;

        virtual std::shared_ptr< EventOutput > eventOutput() const = 0;

        virtual void update( bool block = false ) = 0;
    };
}
