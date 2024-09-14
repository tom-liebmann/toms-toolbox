#pragma once

#include <ttb/ui/elements/Slot.hpp>
#include <ttb/ui/XmlLoader.hpp>
#include <ttb/utils/EventListener.hpp>
#include <ttb/utils/gesture/events.hpp>


namespace ttb::ui
{
    class EventBlocker : public Slot
    {
    public:
        EventBlocker( Root& root );

        ~EventBlocker();

    private:
        //! @copydoc Element::onEvent( Event const& )
        virtual bool onEvent( Event const& event ) override;
    };
}
