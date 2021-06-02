#include <ttb/ui/elements/core/Clickable.hpp>

#include "PriorityListener.hpp"
#include <ttb/ui/Framework.hpp>
#include <ttb/utils/EventManager.hpp>
#include <ttb/utils/gesture/events.hpp>


namespace ttb::ui
{
    Clickable::~Clickable() = default;

    void Clickable::callback( ClickCallback value )
    {
        m_callback = std::move( value );
    }

    void Clickable::child( std::unique_ptr< Element > element )
    {
        wrappedChild( std::move( element ) );
    }

    bool Clickable::onEvent( Event const& event )
    {
        if( !m_callback )
        {
            return false;
        }

        using namespace ttb::event::type;
        switch( event.type() )
        {
            case gst::CURSOR_PRESS_START:
            {
                auto& ev = static_cast< ttb::events::gst::CursorPressStart const& >( event );

                if( range().contains( screenToParent( ev.position() ) ) )
                {
                    m_prioListener =
                        std::make_unique< PriorityListener >( framework().eventManager(), *this );
                    m_prioListener->addType( ttb::event::type::gst::CURSOR_PRESS_ABORT );
                    m_prioListener->addType( ttb::event::type::gst::CURSOR_PRESS_END );
                    m_callback( Action::START, *this );
                    return true;
                }

                return false;
            }

            case gst::CURSOR_PRESS_ABORT:
            {
                if( m_prioListener )
                {
                    m_callback( Action::ABORT, *this );

                    m_prioListener.reset();

                    return true;
                }

                return false;
            }

            case gst::CURSOR_PRESS_END:
            {
                auto& ev = static_cast< ttb::events::gst::CursorPressEnd const& >( event );

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

    Clickable::Clickable( Framework& framework ) : ElementWrapper{ framework }
    {
    }
}
