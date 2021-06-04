#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventType.hpp>
#include <ttb/utils/event/events.hpp>


namespace ttb::events
{
    class PointerPressStart : public TypedEvent< ttb::event::type::POINTER_PRESS_START >
    {
    public:
        using Position = ttb::Vector< float, 2 >;

        PointerPressStart( PointerType pointerType, int pointerId, Position const& position );

        PointerType pointerType() const;

        int pointerId() const;

        Position const& position() const;

    private:
        PointerType m_pointerType;
        int m_pointerId;
        Position m_position;
    };
}


namespace ttb::events
{
    inline PointerPressStart::PointerPressStart( PointerType pointerType,
                                                 int pointerId,
                                                 Position const& position )
        : m_pointerType{ pointerType }, m_pointerId{ pointerId }, m_position( position )
    {
    }

    inline PointerType PointerPressStart::pointerType() const
    {
        return m_pointerType;
    }

    inline int PointerPressStart::pointerId() const
    {
        return m_pointerId;
    }

    inline auto PointerPressStart::position() const -> Position const&
    {
        return m_position;
    }
}
