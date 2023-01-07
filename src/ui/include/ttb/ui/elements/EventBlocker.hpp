#pragma once

#include <ttb/ui/WrappedElement.hpp>
#include <ttb/ui/XmlLoader.hpp>
#include <ttb/utils/EventListener.hpp>
#include <ttb/utils/gesture/events.hpp>


namespace ttb::ui
{
    class EventBlocker : public WrappedElement
    {
    public:
        EventBlocker( Root& root );

        EventBlocker( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader );

        ~EventBlocker();

        void child( Element* element );

    private:
        //! @copydoc Element::onEvent( Event const& )
        virtual bool onEvent( Event const& event ) override;
    };
}
