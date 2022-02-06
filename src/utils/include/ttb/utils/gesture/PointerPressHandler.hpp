#pragma once

#include <ttb/math/Vector.hpp>
#include <ttb/utils/EventListener.hpp>
#include <ttb/utils/EventManager.hpp>
#include <ttb/utils/event/events.hpp>

#include <map>
#include <set>


namespace ttb
{
    class PointerPressHandler : public EventListener
    {
    public:
        using EventCallback = std::function< void( ttb::Event const& ) >;

        PointerPressHandler( EventCallback eventCallback, float moveTolerance );

        virtual bool onEvent( Event const& event ) override;

    private:
        enum class State
        {
            IDLE,
            PRESS,
            DRAG,
            TRANSFORM,
            NONE,
        };

        bool onPointerDown( events::PointerDown const& event );

        bool onPointerUp( events::PointerUp const& event );

        bool onPointerMove( events::PointerMove const& event );

        struct PointerInfo
        {
            ttb::Vector< float, 2 > position;
        };

        EventCallback m_eventCallback;
        float m_moveTolerance;

        std::map< int, PointerInfo > m_activePointers;

        State m_state{ State::IDLE };
    };
}
