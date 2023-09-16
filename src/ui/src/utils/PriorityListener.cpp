#include "PriorityListener.hpp"

#include <ttb/ui/Root.hpp>
#include <ttb/utils/EventManager.hpp>


namespace ttb::ui
{
    PriorityListener::PriorityListener( Root& root, EventListener& listener )
        : m_root{ root }, m_listener{ listener }
    {
        m_root.get().pushPriorityListener( m_listener.get() );
    }

    PriorityListener::~PriorityListener()
    {
        m_root.get().popPriorityListener();
    }

    bool PriorityListener::onEvent( Event const& event )
    {
        return m_listener.get().onEvent( event );
    }
}
