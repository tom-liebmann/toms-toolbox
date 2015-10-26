#pragma once

#include <tg/core/EventListener.hpp>
#include <tg/core/Event.hpp>

#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <set>

// declarations
//=============================================================================

namespace tg
{
    class EventManager
    {
    public:
        class Slot;

        ~EventManager();

        std::shared_ptr< Slot > addListener(
            Event::Type type,
            uint32_t priority,
            std::shared_ptr< EventListener > listener );

        void pushEvent( std::unique_ptr< Event > event );

        void process();

    protected:
        void runEvent( const std::unique_ptr< Event >& event ); 

    private:
        std::map< Event::Type, std::set< Slot*, bool (*)( Slot*, Slot* ) > > m_listener;
        std::list< std::unique_ptr< Event > > m_events;
    };

    

    class EventManager::Slot
    {
    public:
        static bool sort( Slot* lhs, Slot* rhs );
        static void invalidate( Slot* slot );

        Slot(
            uint32_t priority,
            std::shared_ptr< EventListener > listener );

        bool isValid() const;
        uint32_t getPriority() const;
        const std::shared_ptr< EventListener >& getListener() const;

    private:
        bool m_valid;
        uint32_t m_priority;
        std::shared_ptr< EventListener > m_listener;

        friend class EventManager;
    };
}



// definitions
//=============================================================================

namespace tg
{
    inline bool EventManager::Slot::sort( Slot* lhs, Slot* rhs )
    {
        return lhs->getPriority() > rhs->getPriority()
            || ( lhs->getPriority() == rhs->getPriority() && lhs < rhs );
    }

    inline void EventManager::Slot::invalidate( Slot* slot )
    {
        slot->m_valid = false;
    }

    inline EventManager::Slot::Slot(
        uint32_t priority,
        std::shared_ptr< EventListener > listener )
        : m_valid( true )
        , m_priority( priority )
        , m_listener( std::move( listener ) )
    { }

    inline bool EventManager::Slot::isValid() const
    {
        return m_valid;
    }
    
    inline uint32_t EventManager::Slot::getPriority() const
    {
        return m_priority;
    }

    inline const std::shared_ptr< EventListener >& EventManager::Slot::getListener() const
    {
        return m_listener;
    }
}
