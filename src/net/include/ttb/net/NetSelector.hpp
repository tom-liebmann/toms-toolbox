#pragma once

#include <ttb/net/TCPSocket.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/dataIO.hpp>

#include <queue>


namespace ttb
{
    class NetSelector
    {
    public:
        static std::unique_ptr< NetSelector > create();

        virtual ~NetSelector();

        /**
         * Adds a socket to the selector such that its events will be processed and propagated when
         * update is called.
         **/
        virtual void add( std::shared_ptr< TCPSocket > socket ) = 0;

        virtual void remove( std::shared_ptr< TCPSocket > socket ) = 0;

        virtual std::shared_ptr< Provider< SlotType::ACTIVE, Event& > > eventOutput() const = 0;

        virtual void update() = 0;
    };
}
