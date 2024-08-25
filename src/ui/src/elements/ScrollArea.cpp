#include <ttb/ui/elements/ScrollArea.hpp>

#include <fmt/core.h>

#include "utils/PriorityListener.hpp"
#include <ttb/core/window.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/ui/XmlFactory.hpp>
#include <ttb/utils/gesture/events.hpp>


namespace ttb::ui
{
    namespace
    {
        auto const factory = XmlFactory< ScrollArea >{ "scroll" };
    }
}


namespace ttb::ui
{
    ScrollArea::ScrollArea( Root& root ) : ScrollArea{ root, Direction::VERTICAL }
    {
    }

    ScrollArea::ScrollArea( Root& root, Direction direction )
        : Slot{ root }, m_direction{ direction }
    {
    }

    ScrollArea::~ScrollArea() = default;

    void ScrollArea::setScrollOffset( float value )
    {
        m_offset = value;
        setPosition( getPosition() );
    }

    auto ScrollArea::fitWidth( Size const& space ) const -> FitExtent
    {
        if( m_direction == Direction::HORIZONTAL &&
            getWidth().getType() == Extent::Type::MATCH_CHILD )
        {
            throw std::runtime_error{ "Cannot match child in scroll direction" };
        }

        return Slot::fitWidth( space );
    }

    auto ScrollArea::fitHeight( Size const& space ) const -> FitExtent
    {
        if( m_direction == Direction::VERTICAL &&
            getHeight().getType() == Extent::Type::MATCH_CHILD )
        {
            throw std::runtime_error{ "Cannot match child in scroll direction" };
        }

        return Slot::fitHeight( space );
    }

    void ScrollArea::setPosition( Position const& value )
    {
        Slot::setPosition( value );

        if( auto const child = getChild() )
        {
            auto const childSize = child->getSize();

            // TODO Handle direction
            auto const childOffset = [ & ]
            {
                auto const minOffset = 0.0f;
                auto const maxOffset = childSize( 1 ) - getSize()( 1 );

                if( m_offset < minOffset )
                {
                    return m_overScroll * ( 1.0f / ( minOffset - m_offset + 1.0f ) - 1.0f );
                }

                if( m_offset > maxOffset )
                {
                    return maxOffset +
                           m_overScroll * ( 1.0f - 1.0f / ( m_offset - maxOffset + 1.0f ) );
                }

                return m_offset;
            }();

            child->setPosition( { value( 0 ), value( 1 ) - childOffset } );
        }
    }

    void ScrollArea::setSize( Size const& value )
    {
        Element::setSize( value );

        if( auto const child = getChild() )
        {
            auto childSpace = getSize();
            if( m_direction == Direction::HORIZONTAL )
            {
                childSpace( 0 ) = std::numeric_limits< float >::infinity();
            }
            else
            {
                childSpace( 1 ) = std::numeric_limits< float >::infinity();
            }
            auto const childSize = child->finalFit( childSpace );
            child->setSize( childSize );
        }
    }

    bool ScrollArea::onEvent( Event const& event )
    {
        using namespace ttb::event;
        switch( event.type() )
        {
            case type::POINTER_PRESS_START:
                return onPointerPressStart(
                    static_cast< ttb::events::PointerPressStart const& >( event ) );
            case type::DRAG_START:
                return onDragStart( static_cast< ttb::events::DragStart const& >( event ) );
            case type::DRAG_END:
                return onDragEnd( static_cast< ttb::events::DragEnd const& >( event ) );
            case type::DRAG_MOVE:
                return onDragMove( static_cast< ttb::events::DragMove const& >( event ) );
            default:
                return Slot::onEvent( event );
        }
    }

    void ScrollArea::update( float timeDiff )
    {
        if( auto const child = getChild() )
        {
            if( m_prioListener )
            {
                m_velocity *= std::pow( 0.05f, timeDiff );
            }
            else
            {
                auto const minOffset = 0.0f;
                auto const maxOffset =
                    child->getSize()( 1 ) - getSize()( 1 );  // TODO Handle direction

                // Handle overscroll
                if( m_offset < minOffset )
                {
                    m_velocity = 0.0f;
                    setScrollOffset( m_offset * std::pow( m_overScrollFactor, timeDiff ) );
                }
                else if( m_offset > maxOffset )
                {
                    m_velocity = 0.0f;
                    setScrollOffset( maxOffset + ( m_offset - maxOffset ) *
                                                     std::pow( m_overScrollFactor, timeDiff ) );
                }

                // Apply velocity
                if( std::abs( m_velocity ) > 1e-5f )
                {
                    auto newOffset = m_offset + m_velocity * timeDiff;

                    if( newOffset > maxOffset )
                    {
                        newOffset = maxOffset;
                        m_velocity = 0.0f;
                    }

                    if( newOffset < minOffset )
                    {
                        newOffset = minOffset;
                        m_velocity = 0.0f;
                    }

                    setScrollOffset( newOffset );
                }

                if( m_velocity > 0.0f )
                {
                    m_velocity = std::max( 0.0f, m_velocity - 5.0f * timeDiff );
                }
                else if( m_velocity < 0.0f )
                {
                    m_velocity = std::min( 0.0f, m_velocity + 5.0f * timeDiff );
                }
            }
        }

        Slot::update( timeDiff );
    }

    void ScrollArea::render( ttb::State& state ) const
    {
        auto const& window = ttb::Window::instance();
        auto const windowTransform = ttb::mat::transform< float, 2 >(
            { { -1.0f, 1.0f }, { 1.0f, -1.0f } },
            { { 0.0f, 0.0f }, window.viewport().max().as< float >() } );
        auto const& transform =
            windowTransform * ( *state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ) );

        auto const offset = getPosition();
        auto const size = getSize();

        auto const screenTl = transform * ttb::Vector{ offset( 0 ), offset( 1 ), 1.0f };
        auto const screenBr =
            transform * ttb::Vector{ offset( 0 ) + size( 0 ), offset( 1 ) + size( 1 ), 1.0f };

        glEnable( GL_SCISSOR_TEST );
        glScissor( screenTl( 0 ),
                   window.viewport().max( 1 ) - screenBr( 1 ),
                   screenBr( 0 ) - screenTl( 0 ),
                   screenBr( 1 ) - screenTl( 1 ) );

        Slot::render( state );

        glDisable( GL_SCISSOR_TEST );
    }

    bool ScrollArea::onPointerPressStart( ttb::events::PointerPressStart const& event )
    {
        m_velocity = 0.0f;

        return Slot::onEvent( event );
    }

    bool ScrollArea::onDragStart( ttb::events::DragStart const& event )
    {
        auto const offset = getPosition();
        auto const size = getSize();
        auto const eventPos = event.position();

        if( eventPos( 0 ) < offset( 0 ) || eventPos( 0 ) >= offset( 0 ) + size( 0 ) ||
            eventPos( 1 ) < offset( 1 ) || eventPos( 1 ) >= offset( 1 ) + size( 1 ) )
        {
            return Slot::onEvent( event );
        }

        m_dragPos = event.position();

        m_prioListener = std::make_unique< PriorityListener >( getRoot(), *this );

        return true;
    }

    bool ScrollArea::onDragEnd( ttb::events::DragEnd const& event )
    {
        if( m_prioListener )
        {
            m_prioListener.reset();
            return true;
        }

        return Slot::onEvent( event );
    }

    bool ScrollArea::onDragMove( ttb::events::DragMove const& event )
    {
        if( m_prioListener )
        {
            auto const dragAmount = m_dragPos( 1 ) - event.position()( 1 );
            if( std::signbit( dragAmount ) != std::signbit( m_velocity ) )
            {
                m_velocity *= -1.0f;
            }
            m_velocity += std::exp( 8.0f * dragAmount ) - 1.0f;
            // TODO consider direction
            setScrollOffset( m_offset + dragAmount );
            m_dragPos = event.position();
            return true;
        }

        return Slot::onEvent( event );
    }
}
