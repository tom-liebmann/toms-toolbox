#include <ttb/ui/elements/EventBlocker.hpp>

#include "../utils/PriorityListener.hpp"
#include <ttb/ui/Root.hpp>
#include <ttb/ui/XmlFactory.hpp>
#include <ttb/utils/EventManager.hpp>


namespace ttb::ui
{
    namespace
    {
        auto const factory = XmlFactory< EventBlocker >{ "event-blocker" };
    }
}


namespace ttb::ui
{
    EventBlocker::EventBlocker( Root& root ) : Slot{ root }
    {
        setWidth( Extent::Type::MATCH_CHILD );
        setHeight( Extent::Type::MATCH_CHILD );
    }

    EventBlocker::~EventBlocker() = default;

    bool EventBlocker::onEvent( Event const& event )
    {
        [[maybe_unused]] auto result = Slot::onEvent( event );

        return true;
    }
}
