#include <ttb/ui/elements/Clickable.hpp>

#include "PriorityListener.hpp"
#include <ttb/ui/Framework.hpp>
#include <ttb/utils/EventManager.hpp>
#include <ttb/utils/gesture/events.hpp>


namespace ttb::ui
{
    Clickable::Clickable( Framework& framework ) : WrappedElement{ framework }
    {
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

        using namespace ttb::event;
        switch( event.type() )
        {
            case type::POINTER_DOWN:
            {
                if( m_prioListener )
                {
                    return false;
                }

                auto& ev = static_cast< ttb::events::PointerDown const& >( event );
                auto const eventPos = ttb::Vector{ ev.x(), ev.y() }.as< float >();

                if( eventPos( 0 ) >= 0.0f && eventPos( 0 ) < size()( 0 ) && eventPos( 1 ) >= 0.0f &&
                    eventPos( 1 ) < size()( 1 ) )
                {
                    m_pointerId = ev.pointerId();
                    m_prioListener =
                        std::make_unique< PriorityListener >( framework().eventManager(), *this );
                    m_prioListener->addType( type::POINTER_MOVE );
                    m_prioListener->addType( type::POINTER_UP );
                    m_callback( Action::START, *this );
                    return true;
                }

                return false;
            }

            case type::POINTER_MOVE:
            {
                if( !m_prioListener )
                {
                    return false;
                }

                auto& ev = static_cast< ttb::events::PointerMove const& >( event );

                if( ev.pointerId() != m_pointerId )
                {
                    return false;
                }

                auto const eventPos = ttb::Vector{ ev.x(), ev.y() }.as< float >();

                if( eventPos( 0 ) < 0.0f || eventPos( 0 ) >= size()( 0 ) || eventPos( 1 ) < 0.0f ||
                    eventPos( 1 ) >= size()( 1 ) )
                {
                    m_callback( Action::ABORT, *this );
                    m_prioListener.reset();
                }

                return false;
            }

            case type::POINTER_UP:
            {
                if( !m_prioListener )
                {
                    return false;
                }

                auto& ev = static_cast< ttb::events::PointerUp const& >( event );

                if( ev.pointerId() != m_pointerId )
                {
                    return false;
                }

                auto const eventPos = ttb::Vector{ ev.x(), ev.y() }.as< float >();

                if( eventPos( 0 ) >= 0.0f && eventPos( 0 ) < size()( 0 ) && eventPos( 1 ) >= 0.0f &&
                    eventPos( 1 ) < size()( 1 ) )
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

            default:
                return false;
        }
    }
}
