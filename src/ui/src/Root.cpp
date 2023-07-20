#include <ttb/ui/Root.hpp>

#include <ttb/core/uniform.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/utils/EventManager.hpp>
#include <ttb/utils/EventPriority.hpp>


namespace ttb::ui
{
    Root::Root( std::shared_ptr< ttb::resources::Manager > resourceManager, Size const& size )
        : m_resourceManager{ std::move( resourceManager ) }, m_size{ size }
    {
    }

    Root::Root( std::shared_ptr< ttb::resources::Manager > resourceManager )
        : Root{ std::move( resourceManager ), { 1.0f, 1.0f } }
    {
    }

    void Root::setChild( Element* child )
    {
        if( m_child )
        {
            m_child->setParent( nullptr );
        }

        m_child = child;

        if( m_child )
        {
            m_child->setParent( this );
            m_child->size( m_child->fit( m_size ) );
            m_child->offset( {} );
        }
    }

    void Root::setSize( Size const& size )
    {
        m_size = size;

        if( m_child )
        {
            m_child->size( m_child->fit( m_size ) );
        }
    }

    void Root::pushPriorityListener( EventListener& listener )
    {
        m_priorityListeners.push_front( &listener );
    }

    void Root::popPriorityListener()
    {
        m_priorityListeners.pop_front();
    }

    ttb::resources::Manager& Root::getResourceManager()
    {
        return *m_resourceManager;
    }

    void Root::update( float timeDiff )
    {
        if( m_child )
        {
            m_child->update( timeDiff );
        }
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

    bool Root::onEvent( Event const& event )
    {
        // Make a copy to allow concurrent editing
        auto const priorityListeners = m_priorityListeners;
        for( auto const listener : priorityListeners )
        {
            if( listener->onEvent( event ) )
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

    void Root::onChildChanged( Element& /* child */ )
    {
        m_child->size( m_child->fit( m_size ) );
        m_child->offset( {} );
    }
}
