#include "PriorityListener.hpp"

#include <ttb/ui/Framework.hpp>
#include <ttb/ui/elements/Root.hpp>
#include <ttb/utils/EventManager.hpp>


namespace ttb::ui
{
    PriorityListener::PriorityListener( Framework& framework, EventListener& listener )
        : m_framework{ framework }, m_listener{ listener }
    {
        auto const root = m_framework.get().getRoot();

        m_oldListener = root->getPriorityListener();
        root->setPriorityListener( &m_listener.get() );
    }

    PriorityListener::~PriorityListener()
    {
        m_framework.get().getRoot()->setPriorityListener( m_oldListener );
    }

    bool PriorityListener::onEvent( Event const& event )
    {
        return m_listener.get().onEvent( event );
    }
}
