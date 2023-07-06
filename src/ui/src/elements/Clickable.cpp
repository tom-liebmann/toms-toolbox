#include <ttb/ui/elements/Clickable.hpp>

#include "PriorityListener.hpp"
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
    Clickable::Clickable( Root& root ) : WrappedElement{ root }
    {
    }

    Clickable::Clickable( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : WrappedElement{ root }
    {
        if( auto child = node.first_node(); child )
        {
            wrappedChild( loader.loadElement( root, *child ) );
        }
    }

    Clickable::~Clickable() = default;

    void Clickable::callback( ClickCallback value )
    {
        m_callback = std::move( value );
    }

    void Clickable::child( Element* element )
    {
        wrappedChild( std::move( element ) );
    }

    bool Clickable::onEvent( Event const& event )
    {
        if( !m_callback || !wrappedChild() )
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

        auto const offset = this->offset();
        auto const size = this->size();
        auto const& eventPos = event.position();

        if( eventPos( 0 ) < offset( 0 ) || eventPos( 0 ) >= offset( 0 ) + size( 0 ) ||
            eventPos( 1 ) < offset( 1 ) || eventPos( 1 ) >= offset( 1 ) + size( 1 ) )
        {
            return false;
        }

        m_pointerId = event.pointerId();
        m_prioListener = std::make_unique< PriorityListener >( getRoot(), *this );
        m_callback( Action::START, *this );

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

        m_callback( Action::ABORT, *this );
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

        auto const offset = this->offset();
        auto const size = this->size();
        auto const& eventPos = event.position();

        if( eventPos( 0 ) >= offset( 0 ) && eventPos( 0 ) < offset( 0 ) + size( 0 ) &&
            eventPos( 1 ) >= offset( 1 ) && eventPos( 1 ) < offset( 1 ) + size( 1 ) )
        {
            m_callback( Action::END, *this );
        }
        else
        {
            m_callback( Action::ABORT, *this );
        }

        m_prioListener.reset();

        return false;
    }
}
