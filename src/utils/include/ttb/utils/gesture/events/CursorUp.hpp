#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventType.hpp>


namespace ttb::events::gst
{
    class CursorUp : public TypedEvent< ttb::event::type::gst::CURSOR_UP >
    {
    public:
        using Position = ttb::Vector< float, 2 >;

        CursorUp( int cursorId, Position const& position );

        int cursorId() const;

        Position const& position() const;

    private:
        int m_cursorId;
        Position m_position;
    };
}


namespace ttb::events::gst
{
    inline CursorUp::CursorUp( int cursorId, Position const& position )
        : m_cursorId{ cursorId }, m_position( position )
    {
    }

    inline int CursorUp::cursorId() const
    {
        return m_cursorId;
    }

    inline auto CursorUp::position() const -> Position const&
    {
        return m_position;
    }
}
