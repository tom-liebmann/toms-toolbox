#include <ttb/ui/elements/EventBlocker.hpp>

#include "PriorityListener.hpp"
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
    EventBlocker::EventBlocker( Root& root ) : WrappedElement{ root }
    {
    }

    EventBlocker::EventBlocker( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : WrappedElement{ root }
    {
        if( auto child = node.first_node() )
        {
            wrappedChild( loader.loadElement( root, *child ) );
        }
    }

    EventBlocker::~EventBlocker() = default;

    void EventBlocker::child( Element* element )
    {
        wrappedChild( std::move( element ) );
    }

    bool EventBlocker::onEvent( Event const& event )
    {
        if( WrappedElement::onEvent( event ) )
        {
            return true;
        }

        return true;
    }
}
