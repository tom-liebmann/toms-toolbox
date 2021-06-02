#pragma once

#include "CursorType.hpp"
#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventType.hpp>


namespace ttb::events::gst
{
    class CursorDown : public TypedEvent< ttb::event::type::gst::CURSOR_DOWN >
    {
    public:
        using Position = ttb::Vector< float, 2 >;

        CursorDown( CursorType cursorType, int cursorId, Position const& position );

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
    inline CursorDown::CursorDown( CursorType cursorType, int cursorId, Position const& position )
        : m_cursorType{ cursorType }, m_cursorId{ cursorId }, m_position( position )
    {
    }

    inline CursorType CursorDown::cursorType() const
    {
        return m_cursorType;
    }

    inline int CursorDown::cursorId() const
    {
        return m_cursorId;
    }

    inline auto CursorDown::position() const -> Position const&
    {
        return m_position;
    }
}
