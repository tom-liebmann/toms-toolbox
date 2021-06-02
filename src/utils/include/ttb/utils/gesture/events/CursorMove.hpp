#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventType.hpp>


namespace ttb::events::gst
{
    class CursorMove : public TypedEvent< ttb::event::type::gst::CURSOR_MOVE >
    {
    public:
        using Position = ttb::Vector< float, 2 >;

        CursorMove( int cursorId, Position const& position );

        int cursorId() const;

        Position const& position() const;

    private:
        int m_cursorId;
        Position m_position;
    };
}


namespace ttb::events::gst
{
    inline CursorMove::CursorMove( int cursorId, Position const& position )
        : m_cursorId{ cursorId }, m_position( position )
    {
    }

    inline int CursorMove::cursorId() const
    {
        return m_cursorId;
    }

    inline auto CursorMove::position() const -> Position const&
    {
        return m_position;
    }
}
