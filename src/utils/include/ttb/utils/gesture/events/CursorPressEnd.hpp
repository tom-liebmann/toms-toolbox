#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventType.hpp>


namespace ttb::events::gst
{
    class CursorPressEnd : public TypedEvent< ttb::event::type::gst::CURSOR_PRESS_END >
    {
    public:
        using Position = ttb::Vector< float, 2 >;

        CursorPressEnd( int cursorId, Position const& position );

        int cursorId() const;

        Position const& position() const;

    private:
        int m_cursorId;
        Position m_position;
    };
}


namespace ttb::events::gst
{
    inline CursorPressEnd::CursorPressEnd( int cursorId, Position const& position )
        : m_cursorId{ cursorId }, m_position( position )
    {
    }

    inline int CursorPressEnd::cursorId() const
    {
        return m_cursorId;
    }

    inline auto CursorPressEnd::position() const -> Position const&
    {
        return m_position;
    }
}
