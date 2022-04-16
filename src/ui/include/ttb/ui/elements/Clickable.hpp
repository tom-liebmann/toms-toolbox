#pragma once

#include <ttb/ui/WrappedElement.hpp>
#include <ttb/ui/XmlLoader.hpp>
#include <ttb/utils/EventListener.hpp>
#include <ttb/utils/gesture/events.hpp>


namespace ttb::ui
{
    class PriorityListener;
}


namespace ttb::ui
{
    class Clickable : public WrappedElement
    {
    public:
        enum class Action
        {
            START,
            ABORT,
            END
        };

        using ClickCallback = std::function< void( Action, Element& ) >;

        Clickable( Framework& framework );

        Clickable( Framework& framework, rapidxml::xml_node<> const& node, XmlLoader& loader );

        ~Clickable();

        void callback( ClickCallback value );

        void child( Element* element );

    private:
        //! @copydoc Element::onEvent( Event const& )
        virtual bool onEvent( Event const& event ) override;

        bool onPointerDown( ttb::events::PointerDown const& event );

        bool onPointerMove( ttb::events::PointerMove const& event );

        bool onPointerUp( ttb::events::PointerUp const& event );

        ClickCallback m_callback;

        int m_pointerId{ 0 };
        std::unique_ptr< PriorityListener > m_prioListener;
    };
}
