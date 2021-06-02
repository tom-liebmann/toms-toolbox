#pragma once

#include <ttb/utils/EventPriority.hpp>
#include <ttb/utils/EventType.hpp>

#include <unordered_map>
#include <vector>


namespace ttb
{
    class EventListener;
    class Event;
}


namespace ttb
{
    class EventManager
    {
    public:
        void addListener( uint32_t type, uint32_t priority, EventListener& listener );

        void removeListener( uint32_t type, EventListener& listener );

        bool pushEvent( Event const& event ) const;

    private:
        struct Entry
        {
            bool operator<( EventManager::Entry const& rhs ) const;

            uint32_t priority;
            EventListener* listener;
        };


        std::unordered_map< uint32_t, std::vector< Entry > > m_listeners;
    };
}
