#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventType.hpp>


namespace ttb::events
{
    class DragEnd : public TypedEvent< ttb::event::type::DRAG_END >
    {
    public:
        using Position = ttb::Vector< float, 2 >;

        DragEnd( Position const& position );

        Position const& position() const;

        virtual void transform( Transform const& transform,
                                EventCallback const& eventCallback ) const override;

    private:
        Position m_position;
    };
}


namespace ttb::events
{
    inline DragEnd::DragEnd( Position const& position ) : m_position( position )
    {
    }

    inline auto DragEnd::position() const -> Position const&
    {
        return m_position;
    }

    inline void DragEnd::transform( Transform const& transform,
                                    EventCallback const& eventCallback ) const
    {
        eventCallback( DragEnd{ transform( m_position ) } );
    }
}
