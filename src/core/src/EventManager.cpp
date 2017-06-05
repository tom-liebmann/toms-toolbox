#include <ttb/core/EventManager.hpp>

#include <algorithm>

namespace ttb
{
    struct EventManager::SlotCompare
    {
        bool operator()( ttb::EventManager::Slot const& lhs, ttb::EventManager::Slot const& rhs )
        {
            return lhs.m_priority > rhs.m_priority;
        }
    };
}

ttb::EventManager::Slot::Slot( bool valid, uint32_t priority,
                               std::shared_ptr< EventListener > listener )
    : m_valid( valid ), m_priority( priority ), m_listener( std::move( listener ) )
{
}

void ttb::EventManager::addListener( Event::Type type, uint32_t priority,
                                     std::shared_ptr< EventListener > listener )
{
    auto& container = m_listener[ type ];

    container.emplace_back( true, priority, std::move( listener ) );
    std::sort( std::begin( container ), std::end( container ), SlotCompare() );
}

void ttb::EventManager::removeListener( Event::Type type,
                                        std::shared_ptr< EventListener > const& listener )
{
    auto& container = m_listener[ type ];

    auto iter =
        std::find_if( std::begin( container ), std::end( container ),
                      [&listener]( Slot const& slot ) { return slot.m_listener == listener; } );

    if( iter != std::end( container ) )
    {
        iter->m_valid = false;
        iter->m_listener.reset();
    }
}

void ttb::EventManager::runEvent( Event const& event )
{
    auto& container = m_listener[ event.getType() ];

    for( auto iter = std::begin( container ); iter != std::end( container ); )
    {
        if( iter->m_valid )
        {
            if( iter->m_listener->event( event ) )
                break;
            else
                ++iter;
        }
        else
            iter = container.erase( iter );
    }
}

void ttb::EventManager::pushEvent( std::unique_ptr< Event > event )
{
    m_events.push_back( std::move( event ) );
}

void ttb::EventManager::process()
{
    while( !m_events.empty() )
    {
        runEvent( *m_events.front() );
        m_events.pop_front();
    }
}
