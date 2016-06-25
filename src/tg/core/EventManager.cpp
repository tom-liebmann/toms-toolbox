#include "EventManager.hpp"

#include <algorithm>

namespace tg
{
    struct EventManager::SlotCompare
    {
        bool operator()( tg::EventManager::Slot const& lhs, tg::EventManager::Slot const& rhs )
        {
            return lhs.m_priority > rhs.m_priority;
        }
    };
}

tg::EventManager::Slot::Slot( bool valid, uint32_t priority,
                              std::shared_ptr< EventListener > listener )
    : m_valid( valid ), m_priority( priority ), m_listener( std::move( listener ) )
{
}

void tg::EventManager::addListener( Event::Type type, uint32_t priority,
                                    std::shared_ptr< EventListener > listener )
{
    auto& container = m_listener[ type ];

    container.emplace_back( true, priority, std::move( listener ) );
    std::sort( std::begin( container ), std::end( container ), SlotCompare() );
}

void tg::EventManager::removeListener( Event::Type type,
                                       std::shared_ptr< EventListener > const& listener )
{
    auto& container = m_listener[ type ];

    auto iter = std::find_if( std::begin( container ), std::end( container ),
                              [&listener]( Slot const& slot )
                              {
                                  return slot.m_listener == listener;
                              } );

    if( iter != std::end( container ) )
    {
        iter->m_valid = false;
        iter->m_listener.reset();
    }
}

void tg::EventManager::runEvent( std::unique_ptr< Event > const& event )
{
    auto& container = m_listener[ event->getType() ];

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

void tg::EventManager::pushEvent( std::unique_ptr< Event > event )
{
    m_events.push_back( std::move( event ) );
}

void tg::EventManager::process()
{
    while( !m_events.empty() )
    {
        runEvent( m_events.front() );
        m_events.pop_front();
    }
}
