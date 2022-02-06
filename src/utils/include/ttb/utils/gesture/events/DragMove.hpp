#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventType.hpp>


namespace ttb::events
{
    class DragMove : public TypedEvent< ttb::event::type::DRAG_MOVE >
    {
    public:
        using Position = ttb::Vector< float, 2 >;

        DragMove( Position const& position );

        Position const& position() const;

        virtual void transform( Transform const& transform,
                                EventCallback const& eventCallback ) const override;

    private:
        Position m_position;
    };
}


namespace ttb::events
{
    inline DragMove::DragMove( Position const& position ) : m_position( position )
    {
    }

    inline auto DragMove::position() const -> Position const&
    {
        return m_position;
    }

    inline void DragMove::transform( Transform const& transform,
                                     EventCallback const& eventCallback ) const
    {
        eventCallback( DragMove{ transform( m_position ) } );
    }
}
