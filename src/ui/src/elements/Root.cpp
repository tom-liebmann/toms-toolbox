#include <ttb/ui/elements/Root.hpp>

#include <ttb/ui/Framework.hpp>
#include <ttb/utils/EventManager.hpp>
#include <ttb/utils/EventPriority.hpp>


namespace ttb::ui
{
    Root::Root( Framework& framework, Range const& range ) : Element{ framework }
    {
        auto& eventMngr = framework.eventManager();

        using namespace ttb::event;
        eventMngr.addListener( type::POINTER_DOWN, prio::ROOT, *this );
        eventMngr.addListener( type::POINTER_UP, prio::ROOT, *this );
        eventMngr.addListener( type::POINTER_PRESS_START, prio::ROOT, *this );
        eventMngr.addListener( type::POINTER_PRESS_END, prio::ROOT, *this );
        eventMngr.addListener( type::POINTER_PRESS_ABORT, prio::ROOT, *this );
        eventMngr.addListener( type::DRAG_START, prio::ROOT, *this );
        eventMngr.addListener( type::DRAG_END, prio::ROOT, *this );
        eventMngr.addListener( type::DRAG_MOVE, prio::ROOT, *this );
        eventMngr.addListener( type::ZOOM, prio::ROOT, *this );

        Element::range( range );
    }

    Root::~Root()
    {
        auto& eventMngr = framework().eventManager();

        using namespace ttb::event;

        eventMngr.removeListener( type::POINTER_DOWN, *this );
        eventMngr.removeListener( type::POINTER_UP, *this );
        eventMngr.removeListener( type::POINTER_PRESS_START, *this );
        eventMngr.removeListener( type::POINTER_PRESS_END, *this );
        eventMngr.removeListener( type::POINTER_PRESS_ABORT, *this );
        eventMngr.removeListener( type::DRAG_START, *this );
        eventMngr.removeListener( type::DRAG_END, *this );
        eventMngr.removeListener( type::DRAG_MOVE, *this );
        eventMngr.removeListener( type::ZOOM, *this );
    }

    void Root::destroy()
    {
        if( m_child )
        {
            m_child->destroy();
        }
    }

    void Root::child( Element* child )
    {
        if( m_child )
        {
            m_child->destroy();
        }

        m_child = child;
        m_child->parent( this );

        if( m_child )
        {
            m_child->range( m_child->fit( range() ) );
        }
    }

    void Root::range( Range const& range )
    {
        Element::range( range );

        if( m_child )
        {
            m_child->range( m_child->fit( range ) );
        }
    }

    void Root::update( float timeDiff )
    {
        if( m_child )
        {
            m_child->update( timeDiff );
        }
    }

    bool Root::onEvent( Event const& event )
    {
        if( m_child )
        {
            return m_child->onEvent( event );
        }

        return false;
    }

    void Root::render( ttb::State& state ) const
    {
        if( m_child )
        {
            glDisable( GL_DEPTH_TEST );

            m_child->render( state );

            glEnable( GL_DEPTH_TEST );
        }
    }

    void Root::onChildChanged( Element& /* child */ )
    {
        m_child->range( m_child->fit( range() ) );
    }
}
