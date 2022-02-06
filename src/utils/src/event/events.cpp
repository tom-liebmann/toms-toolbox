#include <ttb/utils/event/events.hpp>

#include <ttb/math.hpp>

namespace ttb::events
{
    void PointerDown::transform( Transform const& transform,
                                 EventCallback const& eventCallback ) const
    {
        auto const transformed = transform( ttb::Vector{ m_x, m_y }.as< float >() );
        eventCallback(
            PointerDown{ m_pointerType, m_pointerId, transformed( 0 ), transformed( 1 ) } );
    }


    void PointerUp::transform( Transform const& transform,
                               EventCallback const& eventCallback ) const
    {
        auto const transformed = transform( ttb::Vector{ m_x, m_y }.as< float >() );
        eventCallback( PointerUp{ m_pointerId, transformed( 0 ), transformed( 1 ) } );
    }

    void PointerMove::transform( Transform const& transform,
                                 EventCallback const& eventCallback ) const
    {
        auto const transformed = transform( ttb::Vector{ m_x, m_y }.as< float >() );
        eventCallback( PointerMove{ m_pointerId, transformed( 0 ), transformed( 1 ) } );
    }
}
