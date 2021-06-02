#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventType.hpp>


namespace ttb::events::gst
{
    class DragEnd : public TypedEvent< ttb::event::type::gst::DRAG_END >
    {
    public:
        using Position = ttb::Vector< float, 2 >;

        DragEnd( Position const& position );

        Position const& position() const;

    private:
        Position m_position;
    };
}


namespace ttb::events::gst
{
    inline DragEnd::DragEnd( Position const& position ) : m_position( position )
    {
    }

    inline auto DragEnd::position() const -> Position const&
    {
        return m_position;
    }
}
