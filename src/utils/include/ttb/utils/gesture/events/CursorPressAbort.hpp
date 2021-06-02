#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventType.hpp>


namespace ttb::events::gst
{
    class CursorPressAbort : public TypedEvent< ttb::event::type::gst::CURSOR_PRESS_ABORT >
    {
    public:
        CursorPressAbort( int cursorId );

        int cursorId() const;

    private:
        int m_cursorId;
    };
}


namespace ttb::events::gst
{
    inline CursorPressAbort::CursorPressAbort( int cursorId ) : m_cursorId{ cursorId }
    {
    }

    inline int CursorPressAbort::cursorId() const
    {
        return m_cursorId;
    }
}
