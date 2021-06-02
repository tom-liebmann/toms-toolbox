#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventType.hpp>


namespace ttb::events::gst
{
    class CursorPressStart : public TypedEvent< ttb::event::type::gst::CURSOR_PRESS_START >
    {
    public:
        using Position = ttb::Vector< float, 2 >;

        CursorPressStart( CursorType cursorType, int cursorId, Position const& position );

        CursorType cursorType() const;

        int cursorId() const;

        Position const& position() const;

    private:
        CursorType m_cursorType;
        int m_cursorId;
        Position m_position;
    };
}


namespace ttb::events::gst
{
    inline CursorPressStart::CursorPressStart( CursorType cursorType,
                                               int cursorId,
                                               Position const& position )
        : m_cursorType{ cursorType }, m_cursorId{ cursorId }, m_position( position )
    {
    }

    inline CursorType CursorPressStart::cursorType() const
    {
        return m_cursorType;
    }

    inline int CursorPressStart::cursorId() const
    {
        return m_cursorId;
    }

    inline auto CursorPressStart::position() const -> Position const&
    {
        return m_position;
    }
}
