#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventType.hpp>


namespace ttb::events
{
    class DragStart : public TypedEvent< ttb::event::type::DRAG_START >
    {
    public:
        using Position = ttb::Vector< float, 2 >;

        DragStart( Position const& position );

        Position const& position() const;

        virtual void transform( Transform const& transform,
                                EventCallback const& eventCallback ) const override;

    private:
        Position m_position;
    };
}


namespace ttb::events
{
    inline DragStart::DragStart( Position const& position ) : m_position( position )
    {
    }

    inline auto DragStart::position() const -> Position const&
    {
        return m_position;
    }

    inline void DragStart::transform( Transform const& transform,
                                      EventCallback const& eventCallback ) const
    {
        eventCallback( DragStart{ transform( m_position ) } );
    }
}
