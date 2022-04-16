#include <ttb/ui/elements/Root.hpp>

#include <ttb/core/uniform.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/ui/Framework.hpp>
#include <ttb/utils/EventManager.hpp>
#include <ttb/utils/EventPriority.hpp>


namespace ttb::ui
{
    Root::Root( Framework& framework, Size const& size ) : Element{ framework }
    {
        framework.setRoot( *this );
        this->size( size );
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

        if( m_child )
        {
            m_child->parent( this );
            m_child->size( m_child->fit( size() ) );
            m_child->offset( {} );
        }
    }

    void Root::setPriorityListener( EventListener* listener )
    {
        m_priorityListener = listener;
    }

    EventListener* Root::getPriorityListener() const
    {
        return m_priorityListener;
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
        if( m_priorityListener )
        {
            if( m_priorityListener->onEvent( event ) )
            {
                return true;
            }
        }

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
        m_child->size( m_child->fit( size() ) );
        m_child->offset( {} );
    }
}
