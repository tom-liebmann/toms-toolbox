#include <ttb/ui/elements/core/Root.hpp>

#include <ttb/ui/Framework.hpp>
#include <ttb/utils/EventManager.hpp>
#include <ttb/utils/EventPriority.hpp>


namespace ttb::ui
{
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

    void Root::child( std::unique_ptr< Element > child )
    {
        if( m_child )
        {
            m_child->destroy();
        }

        m_child = std::move( child );
        m_child->parent( *this );

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

            auto u =
                state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ).push( m_transform );

            m_child->render( state );

            glEnable( GL_DEPTH_TEST );
        }
    }

    void Root::onChildChanged( Element& /* child */ )
    {
        m_child->range( m_child->fit( range() ) );
    }

    Element* Root::child( std::string const& /* id */ )
    {
        return m_child.get();
    }

    Element const* Root::child( std::string const& /* id */ ) const
    {
        return m_child.get();
    }

    Root::Root( Framework& framework, Range const& srcRange, Range const& dstRange )
        : Element{ framework }, m_transform( ttb::mat::transform( dstRange, srcRange ) )
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

        Element::range( dstRange );
    }
}
