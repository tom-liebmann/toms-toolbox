#include <ttb/ui/elements/Switch.hpp>


namespace ttb::ui
{
    Switch::Switch( Framework& framework ) : Element{ framework }
    {
    }

    void Switch::set( Index index, Element* child )
    {
        if( child )
        {
            child->parent( this );

            auto const iter = m_children.find( index );
            if( iter != std::end( m_children ) )
            {
                iter->second->destroy();
            }

            if( index == m_activeIndex )
            {
                m_activeChild = child;
            }

            m_children.insert_or_assign( index, child );

            if( index == m_activeIndex )
            {
                changed();
            }
        }
        else
        {
            auto const iter = m_children.find( index );
            if( iter != std::end( m_children ) )
            {
                iter->second->destroy();
                m_children.erase( iter );
            }
        }
    }

    void Switch::activate( Index index )
    {
        if( index == m_activeIndex )
        {
            return;
        }

        m_activeIndex = index;

        auto const iter = m_children.find( index );

        if( iter != std::end( m_children ) )
        {
            m_activeChild = iter->second;
        }
        else
        {
            m_activeChild = nullptr;
        }

        changed();
    }

    void Switch::destroy()
    {
        if( m_activeChild )
        {
            m_activeChild->destroy();
        }
    }

    auto Switch::fit( Size const& size ) -> Size
    {
        if( m_activeChild )
        {
            return Element::fit( m_activeChild->fit( size ) );
        }

        return Element::fit( { 0.0f, 0.0f } );
    }

    void Switch::update( float timeDiff )
    {
        if( m_activeChild )
        {
            m_activeChild->update( timeDiff );
        }
    }

    void Switch::render( ttb::State& state ) const
    {
        if( m_activeChild )
        {
            m_activeChild->render( state );
        }
    }

    bool Switch::onEvent( Event const& event )
    {
        if( m_activeChild )
        {
            return m_activeChild->onEvent( event );
        }

        return false;
    }

    void Switch::onChildChanged( Element& child )
    {
        if( m_activeChild == &child )
        {
            changed();
        }
    }
}
