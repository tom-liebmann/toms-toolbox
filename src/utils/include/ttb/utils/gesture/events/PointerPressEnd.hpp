#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventType.hpp>


namespace ttb::events
{
    class PointerPressEnd : public TypedEvent< ttb::event::type::POINTER_PRESS_END >
    {
    public:
        using Position = ttb::Vector< float, 2 >;

        PointerPressEnd( int pointerId, Position const& position );

        int pointerId() const;

        Position const& position() const;

        virtual void transform( Transform const& transform,
                                EventCallback const& eventCallback ) const override;

    private:
        int m_pointerId;
        Position m_position;
    };
}


namespace ttb::events
{
    inline PointerPressEnd::PointerPressEnd( int pointerId, Position const& position )
        : m_pointerId{ pointerId }, m_position( position )
    {
    }

    inline int PointerPressEnd::pointerId() const
    {
        return m_pointerId;
    }

    inline auto PointerPressEnd::position() const -> Position const&
    {
        return m_position;
    }

    inline void PointerPressEnd::transform( Transform const& transform,
                                            EventCallback const& eventCallback ) const
    {
        eventCallback( PointerPressEnd{ m_pointerId, transform( m_position ) } );
    }
}
