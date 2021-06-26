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
            case type::POINTER_PRESS_START:
            {
                auto& ev = static_cast< ttb::events::PointerPressStart const& >( event );

                if( range().contains( screenToParent( ev.position() ) ) )
                {
                    m_prioListener =
                        std::make_unique< PriorityListener >( framework().eventManager(), *this );
                    m_prioListener->addType( type::POINTER_PRESS_ABORT );
                    m_prioListener->addType( type::POINTER_PRESS_END );
                    m_callback( Action::START, *this );
                    return true;
                }

                return false;
            }

            case type::POINTER_PRESS_ABORT:
            {
                if( m_prioListener )
                {
                    m_callback( Action::ABORT, *this );

                    m_prioListener.reset();

                    return true;
                }

                return false;
            }

            case type::POINTER_PRESS_END:
            {
                auto& ev = static_cast< ttb::events::PointerPressEnd const& >( event );

                if( m_prioListener )
                {
                    if( range().contains( screenToParent( ev.position() ) ) )
                    {
                        m_callback( Action::END, *this );
                    }
                    else
                    {
                        m_callback( Action::ABORT, *this );
                    }

                    m_prioListener.reset();

                    return true;
                }

                return false;
            }

            default:
                return false;
        }
    }
}
