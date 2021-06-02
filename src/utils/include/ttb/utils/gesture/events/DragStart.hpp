#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventType.hpp>


namespace ttb::events::gst
{
    class DragStart : public TypedEvent< ttb::event::type::gst::DRAG_START >
    {
    public:
        using Position = ttb::Vector< float, 2 >;

        DragStart( Position const& position );

        Position const& position() const;

    private:
        Position m_position;
    };
}


namespace ttb::events::gst
{
    inline DragStart::DragStart( Position const& position ) : m_position( position )
    {
    }

    inline auto DragStart::position() const -> Position const&
    {
        return m_position;
    }
}
