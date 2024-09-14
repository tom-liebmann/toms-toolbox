#include <ttb/ui/elements/Clickable.hpp>

#include "../utils/PriorityListener.hpp"
#include <ttb/ui/Root.hpp>
#include <ttb/ui/XmlFactory.hpp>
#include <ttb/utils/EventManager.hpp>


namespace ttb::ui
{
    namespace
    {
        auto const factory = XmlFactory< Clickable >{ "clickable" };
    }
}


namespace ttb::ui
{
    Clickable::Clickable( Root& root ) : Slot{ root }
    {
    }

    Clickable::~Clickable() = default;

    void Clickable::setPressCallback( Callback callback )
    {
        m_callback = std::move( callback );
    }

    void Clickable::onPressStart()
    {
        if( m_callback )
        {
            m_callback( Action::START );
        }
    }

    void Clickable::onPressEnd()
    {
        if( m_callback )
        {
            m_callback( Action::END );
        }
    }

    void Clickable::onPressAbort()
    {
        if( m_callback )
        {
            m_callback( Action::ABORT );
        }
    }

    bool Clickable::onEvent( Event const& event )
    {
        if( !getChild() )
        {
            return false;
        }

        switch( event.type() )
        {
            case ttb::event::type::POINTER_PRESS_START:
                return onPointerPressStart(
                    static_cast< ttb::events::PointerPressStart const& >( event ) );

            case ttb::event::type::POINTER_PRESS_ABORT:
                return onPointerPressAbort(
                    static_cast< ttb::events::PointerPressAbort const& >( event ) );

            case ttb::event::type::POINTER_PRESS_END:
                return onPointerPressEnd(
                    static_cast< ttb::events::PointerPressEnd const& >( event ) );

            default:
                return false;
        }
    }

    bool Clickable::onPointerPressStart( ttb::events::PointerPressStart const& event )
    {
        if( m_prioListener )
        {
            return false;
        }

        auto const& pos = getPosition();
        auto const& size = getSize();
        auto const& eventPos = event.position();

        if( eventPos( 0 ) < pos( 0 ) || eventPos( 0 ) >= pos( 0 ) + size( 0 ) ||
            eventPos( 1 ) < pos( 1 ) || eventPos( 1 ) >= pos( 1 ) + size( 1 ) )
        {
            return false;
        }

        m_pointerId = event.pointerId();
        m_prioListener = std::make_unique< PriorityListener >( getRoot(), *this );
        onPressStart();

        return true;
    }

    bool Clickable::onPointerPressAbort( ttb::events::PointerPressAbort const& event )
    {
        if( !m_prioListener )
        {
            return false;
        }

        if( event.pointerId() != m_pointerId )
        {
            return false;
        }

        onPressAbort();
        m_prioListener.reset();

        return false;
    }

    bool Clickable::onPointerPressEnd( ttb::events::PointerPressEnd const& event )
    {
        if( !m_prioListener )
        {
            return false;
        }

        if( event.pointerId() != m_pointerId )
        {
            return false;
        }

        auto const& pos = getPosition();
        auto const& size = getSize();
        auto const& eventPos = event.position();

        if( eventPos( 0 ) >= pos( 0 ) && eventPos( 0 ) < pos( 0 ) + size( 0 ) &&
            eventPos( 1 ) >= pos( 1 ) && eventPos( 1 ) < pos( 1 ) + size( 1 ) )
        {
            onPressEnd();
        }
        else
        {
            onPressAbort();
        }

        m_prioListener.reset();

        return false;
    }
}
