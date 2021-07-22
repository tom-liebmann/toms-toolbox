#include <ttb/ui/elements/ScrollArea.hpp>

#include "PriorityListener.hpp"
#include <ttb/core/window.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/ui/Framework.hpp>
#include <ttb/ui/WrappedElement.hpp>
#include <ttb/utils/gesture/events.hpp>


namespace ttb::ui
{
    class ScrollArea::ScrollableArea : public WrappedElement
    {
    public:
        ScrollableArea( Framework& framework );

        void child( Element* element );
    };
}


namespace ttb::ui
{
    ScrollArea::ScrollArea( Framework& framework, Direction direction )
        : Element{ framework }
        , m_direction{ direction }
        , m_child{ std::make_unique< ScrollableArea >( framework ) }
        , m_transform( ttb::mat::identity< float, 3 >() )
    {
        m_child->parent( this );
    }

    ScrollArea::~ScrollArea() = default;

    void ScrollArea::child( Element* element )
    {
        m_child->child( element );
    }

    void ScrollArea::range( Range const& range )
    {
        Element::range( range );
        m_transform( 0, 2 ) = range.min( 0 );
        m_transform( 1, 2 ) = range.min( 1 );

        if( !m_child )
        {
            return;
        }

        offset( m_offset );
    }

    void ScrollArea::destroy()
    {
        m_child->destroy();
    }

    void ScrollArea::render( ttb::State& state ) const
    {
        auto const& windowSize = Window::instance().size();

        auto const& range = this->range();
        auto min = localToScreen( range.min() );
        auto max = localToScreen( range.max() );
        min( 0 ) = min( 0 ) * windowSize( 0 );
        min( 1 ) = min( 1 ) * windowSize( 0 );
        max( 0 ) = max( 0 ) * windowSize( 0 );
        max( 1 ) = max( 1 ) * windowSize( 0 );
        auto const viewport = ttb::Viewport{ static_cast< GLint >( min( 0 ) ),
                                             windowSize( 1 ) - static_cast< GLint >( max( 1 ) ),
                                             static_cast< GLsizei >( max( 0 ) - min( 0 ) ),
                                             static_cast< GLsizei >( max( 1 ) - min( 1 ) ) };

        glEnable( GL_SCISSOR_TEST );
        glScissor( viewport.getX(), viewport.getY(), viewport.getWidth(), viewport.getHeight() );
        {
            auto const u =
                state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ).push( m_transform );

            m_child->render( state );
        }
        glDisable( GL_SCISSOR_TEST );
    }

    auto ScrollArea::fit( Range const& space ) -> Range
    {
        return space;
    }

    void ScrollArea::update( float timeDiff )
    {
        m_child->update( timeDiff );
    }

    bool ScrollArea::onEvent( Event const& event )
    {
        using namespace ttb::event;
        switch( event.type() )
        {
            case type::POINTER_PRESS_START:
            {
                auto& ev = static_cast< ttb::events::PointerPressStart const& >( event );

                auto const localPos = screenToLocal( ev.position() );
                if( localPos( 0 ) < 0.0f || localPos( 0 ) >= 1.0f || localPos( 1 ) < 0.0f ||
                    localPos( 1 ) >= 1.0f )
                {
                    return false;
                }

                return m_child->onEvent( event );
            }

            case type::DRAG_START:
            {
                auto& ev = static_cast< ttb::events::DragStart const& >( event );

                auto const localPos = screenToLocal( ev.position() );
                if( localPos( 0 ) < 0.0f || localPos( 0 ) >= 1.0f || localPos( 1 ) < 0.0f ||
                    localPos( 1 ) >= 1.0f )
                {
                    return false;
                }

                m_dragPos = ev.position();

                m_prioListener =
                    std::make_unique< PriorityListener >( framework().eventManager(), *this );
                m_prioListener->addType( type::DRAG_END );
                m_prioListener->addType( type::DRAG_MOVE );

                return true;
            }

            case type::DRAG_END:
            {
                if( m_prioListener )
                {
                    m_prioListener.reset();
                    return true;
                }

                return false;
            }

            case type::DRAG_MOVE:
            {
                if( m_prioListener )
                {
                    auto& ev = static_cast< ttb::events::DragMove const& >( event );
                    offset( m_offset + ev.position()( 1 ) - m_dragPos( 1 ) );
                    m_dragPos = ev.position();
                    return true;
                }

                return false;
            }

            default:
                return m_child->onEvent( event );
        }
    }


    void ScrollArea::onChildChanged( Element& /* child */ )
    {
        range( range() );
    }

    void ScrollArea::offset( float value )
    {
        auto const& range = this->range();
        m_offset = value;

        auto const childSpace = m_child->fit( { { 0.0f, 0.0f }, range.extent() } );

        m_offset = std::max( range.extent( 1 ) - childSpace.extent( 1 ), m_offset );
        m_offset = std::min( 0.0f, m_offset );

        m_child->range(
            { { 0.0f, m_offset }, { childSpace.extent( 0 ), childSpace.extent( 1 ) + m_offset } } );
    }
}


namespace ttb::ui
{
    ScrollArea::ScrollableArea::ScrollableArea( Framework& framework ) : WrappedElement{ framework }
    {
    }

    void ScrollArea::ScrollableArea::child( Element* element )
    {
        wrappedChild( element );
    }
}
