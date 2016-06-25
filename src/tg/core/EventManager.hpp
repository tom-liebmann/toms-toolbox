#pragma once

#include <tg/core/EventListener.hpp>
#include <tg/core/Event.hpp>

#include <cstdint>
#include <list>
#include <unordered_map>
#include <vector>
#include <memory>

// declarations
//=============================================================================

namespace tg
{
    class EventManager
    {
    public:
        /// Adds an event listener to the manager.
        /**
         * \param type Type of the event the listener wants to be informed about.
         * \param priority The priority specifies the order the listeners get informed in.
         * \param listener The listener that gets added to the set.
         */
        void addListener( Event::Type type, uint32_t priority,
                          std::shared_ptr< EventListener > listener );

        /// Removes a listener.
        /**
         * \param type The event type the listener currently is listening to.
         * \param listener The listener that gets removed.
         */
        void removeListener( Event::Type type, std::shared_ptr< EventListener > const& listener );

        /// Pushes an event onto the event queue so that all registered listeners get informed.
        /**
         * \param event The event that gets pushed.
         */
        void pushEvent( std::unique_ptr< Event > event );

        /// Informs all listeners about events on the queue and clears the queue afterwards.
        void process();

    protected:
        void runEvent( std::unique_ptr< Event > const& event );

    private:
        struct SlotCompare;

        struct Slot
        {
            Slot( bool valid, uint32_t priority, std::shared_ptr< EventListener > listener );

            bool m_valid;
            uint32_t m_priority;
            std::shared_ptr< EventListener > m_listener;
        };

        std::unordered_map< Event::Type, std::vector< Slot > > m_listener;
        std::list< std::unique_ptr< Event > > m_events;
    };
}
