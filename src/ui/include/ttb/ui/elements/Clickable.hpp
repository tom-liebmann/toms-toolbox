#pragma once

#include <ttb/ui/elements/Slot.hpp>
#include <ttb/ui/xml_loading.hpp>
#include <ttb/utils/EventListener.hpp>
#include <ttb/utils/gesture/events.hpp>


namespace ttb::ui
{
    class PriorityListener;
}


namespace ttb::ui
{
    class Clickable : public Slot
    {
    public:
        Clickable( Root& root );

        ~Clickable();

        enum class Action
        {
            START,
            END,
            ABORT,
        };

        using Callback = std::function< void( Action ) >;

        void setPressCallback( Callback callback );

    protected:
        virtual void onPressStart();

        virtual void onPressEnd();

        virtual void onPressAbort();

    private:
        //! @copydoc Element::onEvent( Event const& )
        virtual bool onEvent( Event const& event ) override;

        bool onPointerPressStart( ttb::events::PointerPressStart const& event );

        bool onPointerPressAbort( ttb::events::PointerPressAbort const& event );

        bool onPointerPressEnd( ttb::events::PointerPressEnd const& event );

        int m_pointerId{ 0 };
        std::unique_ptr< PriorityListener > m_prioListener;

        Callback m_callback;
    };
}
