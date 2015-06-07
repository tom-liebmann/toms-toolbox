#include "EventManager.hpp"

tg::EventManager::~EventManager()
{
    for( auto& listenerSet : m_listener )
    {
        for( auto& listener : listenerSet.second )
        {
            if( listener->isValid() )
                listener->m_valid = false;
            else
                delete listener;
        }
    }
}

std::shared_ptr< tg::EventManager::Slot > tg::EventManager::addListener(
    Event::Type type,
    uint32_t priority,
    std::shared_ptr< EventListener > listener )
{
    Slot* slot = new Slot( priority, std::move( listener ) );

    auto iter = m_listener.find( type );
    if( iter == m_listener.end() )
    {
        std::set< Slot*, bool (*)( Slot*, Slot* ) > newSet( Slot::sort );
        newSet.insert( slot );
        m_listener[ type ] = std::move( newSet );
    }
    else
        iter->second.insert( slot );

    return std::shared_ptr< Slot >( slot, Slot::invalidate );
}

void tg::EventManager::runEvent( const std::unique_ptr< Event >& event )
{
    auto iter = m_listener.find( event->getType() );
    if( iter != m_listener.end() )
    {
        for( auto i = iter->second.begin(); i != iter->second.end(); )
        {
            if( (*i)->isValid() )
            {
                if( (*i)->getListener()->event( event ) )
                    break;
                ++i;
            }
            else
            {
                delete *i;
                i = iter->second.erase( i );
            }
        }
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
