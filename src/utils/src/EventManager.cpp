#include <ttb/utils/EventManager.hpp>

#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventListener.hpp>

#include <algorithm>


namespace ttb
{
    void EventManager::addListener( uint32_t type, uint32_t priority, EventListener& listener )
    {
        auto& listeners = m_listeners[ type ];

        listeners.push_back( Entry{ priority, &listener } );

        std::sort( std::begin( listeners ), std::end( listeners ) );
    }

    void EventManager::removeListener( uint32_t type, EventListener& listener )
    {
        auto iter = m_listeners.find( type );

        if( iter != std::end( m_listeners ) )
        {
            auto& listeners = iter->second;

            auto entryIter =
                std::find_if( std::begin( listeners ),
                              std::end( listeners ),
                              [ & ]( auto const& value ) { return value.listener == &listener; } );

            if( entryIter != std::end( listeners ) )
            {
                listeners.erase( entryIter );
            }
        }
    }

    bool EventManager::pushEvent( Event const& event ) const
    {
        auto iter = m_listeners.find( event.type() );

        if( iter != std::end( m_listeners ) )
        {
            auto& listeners = iter->second;

            for( auto& listener : listeners )
            {
                if( listener.listener->onEvent( event ) )
                {
                    return true;
                }
            }
        }

        return false;
    }


    bool EventManager::Entry::operator<( EventManager::Entry const& rhs ) const
    {
        return priority > rhs.priority;
    }
}
