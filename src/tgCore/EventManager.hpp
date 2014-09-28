#pragma once

#include <tgCore/EventListener.hpp>

#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <set>

namespace tgCore
{
    template< class T >
    class EventManager
    {
        public:
            class Slot
            {
                public:
                    static bool sort( Slot* slot1, Slot* slot2 );
                    static void invalidate( Slot* slot );

                    Slot(
                        uint32_t priority,
                        std::shared_ptr< EventListener< T > > listener );

                    bool isValid() const;
                    uint32_t getPriority() const;
                    const std::shared_ptr< EventListener< T > >& getListener() const;

                private:
                    bool m_valid;
                    uint32_t m_priority;
                    std::shared_ptr< EventListener< T > > m_listener;
            };

            std::shared_ptr< Slot > addListener(
                typename T::Type type,
                uint32_t priority,
                std::shared_ptr< EventListener< T > > listener );

            void pushEvent( std::unique_ptr< T > event );

            void process( float timeFactor );

        private:
            std::list< std::unique_ptr< T > > m_events;
            std::map< typename T::Type, std::set< Slot*, bool (*)( Slot*, Slot* ) > > m_listener;
    };



    template< class T >
    std::shared_ptr< typename EventManager< T >::Slot > EventManager< T >::addListener(
        typename T::Type type,
        uint32_t priority,
        std::shared_ptr< EventListener< T > > listener )
    {
        Slot* slot = new Slot( priority, std::move( listener ) );

        auto iter = m_listener.find( type );
        if( iter == m_listener.end() )
            m_listener[ type ] = std::set< Slot*, bool (*)( Slot*, Slot* ) >{ slot, Slot::sort };
        else
            iter->second.insert( slot );

        return std::shared_ptr< Slot >( slot, Slot::invalidate );
    }

    template< class T >
    void EventManager< T >::pushEvent( std::unique_ptr< T > event )
    {
        m_events.push_back( std::move( event ) );
    }

    template< class T >
    void EventManager< T >::process( float timeFactor )
    {
        while( !m_events.empty() )
        {
            auto& event = m_events.front();

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

            m_events.pop_front();
        }
    }

    template< class T >
    bool EventManager< T >::Slot::sort( Slot* slot1, Slot* slot2 )
    {
        return slot1->getPriority() > slot2->getPriority();
    }

    template< class T >
    void EventManager< T >::Slot::invalidate( Slot* slot )
    {
        slot->m_valid = false;
    }

    template< class T >
    inline EventManager< T >::Slot::Slot(
        uint32_t priority,
        std::shared_ptr< EventListener< T > > listener )
        : m_valid( true )
        , m_priority( priority )
        , m_listener( std::move( listener ) )
    { }

    template< class T >
    inline bool EventManager< T >::Slot::isValid() const
    {
        return m_valid;
    }
    
    template< class T >
    inline uint32_t EventManager< T >::Slot::getPriority() const
    {
        return m_priority;
    }

    template< class T >
    inline const std::shared_ptr< EventListener< T > >& EventManager< T >::Slot::getListener() const
    {
        return m_listener;
    }
}
