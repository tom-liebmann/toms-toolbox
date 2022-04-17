#include <ttb/ui/elements/Clickable.hpp>

#include "PriorityListener.hpp"
#include <ttb/ui/Framework.hpp>
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
    Clickable::Clickable( Framework& framework ) : WrappedElement{ framework }
    {
    }

    Clickable::Clickable( Framework& framework,
                          rapidxml::xml_node<> const& node,
                          XmlLoader& loader )
        : WrappedElement{ framework }
    {
        if( auto child = node.first_node(); child )
        {
            wrappedChild( loader.loadElement( framework, *child ) );
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
            case ttb::event::type::POINTER_DOWN:
                return onPointerDown( static_cast< ttb::events::PointerDown const& >( event ) );

            case ttb::event::type::POINTER_MOVE:
                return onPointerMove( static_cast< ttb::events::PointerMove const& >( event ) );

            case ttb::event::type::POINTER_UP:
                return onPointerUp( static_cast< ttb::events::PointerUp const& >( event ) );

            default:
                return false;
        }
    }

    bool Clickable::onPointerDown( ttb::events::PointerDown const& event )
    {
        if( m_prioListener )
        {
            return false;
        }

        auto const offset = this->offset();
        auto const size = this->size();
        auto const eventPos = ttb::Vector{ event.x(), event.y() }.as< float >();

        if( eventPos( 0 ) < offset( 0 ) || eventPos( 0 ) >= offset( 0 ) + size( 0 ) ||
            eventPos( 1 ) < offset( 1 ) || eventPos( 1 ) >= offset( 1 ) + size( 1 ) )
        {
            return false;
        }

        m_pointerId = event.pointerId();
        m_prioListener = std::make_unique< PriorityListener >( framework(), *this );
        m_callback( Action::START, *this );

        return true;
    }

    bool Clickable::onPointerMove( ttb::events::PointerMove const& event )
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
        auto const eventPos = ttb::Vector{ event.x(), event.y() }.as< float >();

        if( eventPos( 0 ) < offset( 0 ) || eventPos( 0 ) >= offset( 0 ) + size( 0 ) ||
            eventPos( 1 ) < offset( 1 ) || eventPos( 1 ) >= offset( 1 ) + size( 1 ) )
        {
            m_callback( Action::ABORT, *this );
            m_prioListener.reset();
        }

        return false;
    }

    bool Clickable::onPointerUp( ttb::events::PointerUp const& event )
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
        auto const eventPos = ttb::Vector{ event.x(), event.y() }.as< float >();

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
