#include <ttb/ui/elements/ScrollArea.hpp>

#include <fmt/core.h>

#include "PriorityListener.hpp"
#include <ttb/core/window.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/ui/WrappedElement.hpp>
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
    ScrollArea::ScrollArea( Root& root, Direction direction )
        : WrappedElement{ root }, m_direction{ direction }
    {
    }

    ScrollArea::ScrollArea( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : WrappedElement{ root }
    {
        if( auto child = node.first_node(); child )
        {
            wrappedChild( loader.loadElement( root, *child ) );
        }
    }

    ScrollArea::~ScrollArea() = default;

    void ScrollArea::setChild( Element* element )
    {
        wrappedChild( element );
    }

    void ScrollArea::setScrollOffset( float value )
    {
        m_offset = value;
        offset( offset() );
    }

    auto ScrollArea::fit( Size const& space ) -> Size
    {
        if( auto const child = wrappedChild(); child )
        {
            // TODO Handle direction
            auto childSpace = space;
            childSpace( 1 ) = std::numeric_limits< float >::infinity();
            auto const childSize = child->fit( childSpace );

            auto mySize = childSize;
            mySize( 1 ) = std::min( space( 1 ), childSize( 1 ) );
            return mySize;
        }

        return space;
    }

    void ScrollArea::offset( Offset const& value )
    {
        Element::offset( value );

        if( auto const child = wrappedChild(); child )
        {
            auto const childSize = child->size();

            // TODO Handle direction
            auto const childOffset = [ & ]
            {
                auto const minOffset = 0.0f;
                auto const maxOffset = childSize( 1 ) - size()( 1 );

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

            child->offset( { value( 0 ), value( 1 ) - childOffset } );
        }
    }

    void ScrollArea::size( Size const& value )
    {
        Element::size( value );

        if( auto const child = wrappedChild(); child )
        {
            auto childSpace = value;
            childSpace( 1 ) = std::numeric_limits< float >::infinity();
            auto const childSize = child->fit( childSpace );
            child->size( childSize );
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
                return WrappedElement::onEvent( event );
        }
    }

    void ScrollArea::update( float timeDiff )
    {
        if( auto const child = wrappedChild(); child )
        {
            if( m_prioListener )
            {
                m_velocity *= std::pow( 0.05f, timeDiff );
            }
            else
            {
                auto const minOffset = 0.0f;
                auto const maxOffset = child->size()( 1 ) - size()( 1 );  // TODO Handle direction

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

        WrappedElement::update( timeDiff );
    }

    void ScrollArea::render( ttb::State& state ) const
    {
        auto const& window = ttb::Window::instance();
        auto const windowTransform = ttb::mat::transform< float, 2 >(
            { { -1.0f, 1.0f }, { 1.0f, -1.0f } },
            { { 0.0f, 0.0f }, window.viewport().max().as< float >() } );
        auto const& transform =
            windowTransform * ( *state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ) );

        auto const offset = this->offset();
        auto const size = this->size();

        auto const screenTl = transform * ttb::Vector{ offset( 0 ), offset( 1 ), 1.0f };
        auto const screenBr =
            transform * ttb::Vector{ offset( 0 ) + size( 0 ), offset( 1 ) + size( 1 ), 1.0f };

        glEnable( GL_SCISSOR_TEST );
        glScissor( screenTl( 0 ),
                   window.viewport().max( 1 ) - screenBr( 1 ),
                   screenBr( 0 ) - screenTl( 0 ),
                   screenBr( 1 ) - screenTl( 1 ) );

        WrappedElement::render( state );

        glDisable( GL_SCISSOR_TEST );
    }

    bool ScrollArea::onPointerPressStart( ttb::events::PointerPressStart const& event )
    {
        m_velocity = 0.0f;

        return WrappedElement::onEvent( event );
    }

    bool ScrollArea::onDragStart( ttb::events::DragStart const& event )
    {
        auto const offset = this->offset();
        auto const size = this->size();
        auto const eventPos = event.position();

        if( eventPos( 0 ) < offset( 0 ) || eventPos( 0 ) >= offset( 0 ) + size( 0 ) ||
            eventPos( 1 ) < offset( 1 ) || eventPos( 1 ) >= offset( 1 ) + size( 1 ) )
        {
            return WrappedElement::onEvent( event );
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

        return WrappedElement::onEvent( event );
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

        return WrappedElement::onEvent( event );
    }
}
