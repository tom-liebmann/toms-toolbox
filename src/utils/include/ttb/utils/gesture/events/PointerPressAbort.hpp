#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventType.hpp>


namespace ttb::events
{
    class PointerPressAbort : public TypedEvent< ttb::event::type::POINTER_PRESS_ABORT >
    {
    public:
        PointerPressAbort( int pointerId );

        int pointerId() const;

    private:
        int m_pointerId;
    };
}


namespace ttb::events
{
    inline PointerPressAbort::PointerPressAbort( int pointerId ) : m_pointerId{ pointerId }
    {
    }

    inline int PointerPressAbort::pointerId() const
    {
        return m_pointerId;
    }
}
