#pragma once

#include <ttb/utils/EventPriority.hpp>
#include <ttb/utils/EventType.hpp>


namespace ttb
{
    class EventListener;
}


namespace ttb
{
    class EventManager
    {
    public:
        void addListener( uint32_t type, uint32_t priority, EventListener& listener );

        void removeListener( uint32_t type, EventListener& listener );

        bool pushEvent( Event& event );

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
