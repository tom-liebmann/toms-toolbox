#include "PriorityListener.hpp"

#include <ttb/utils/EventManager.hpp>


namespace ttb::ui
{
    PriorityListener::PriorityListener( EventManager& manager, EventListener& listener )
        : m_manager{ manager }, m_listener{ listener }
    {
    }

    void PriorityListener::addType( uint32_t type )
    {
        m_manager.get().addListener( type, ttb::event::prio::ui::PRIO, *this );

        m_types.push_back( type );
    }

    PriorityListener::~PriorityListener()
    {
        for( auto const& type : m_types )
        {
            m_manager.get().removeListener( type, *this );
        }
    }

    bool PriorityListener::onEvent( Event& event )
    {
        return m_listener.get().onEvent( event );
    }
}
