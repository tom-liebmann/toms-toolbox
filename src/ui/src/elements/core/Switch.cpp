#include <ttb/ui/elements/core/Switch.hpp>


namespace ttb::ui
{
    std::unique_ptr< Element > Switch::release( Index index )
    {
        auto const iter = m_children.find( index );

        if( iter == std::end( m_children ) )
        {
            return std::unique_ptr< Element >{};
        }

        if( index == m_activeIndex )
        {
            m_activeChild = nullptr;
        }

        std::unique_ptr< Element > result = std::move( iter->second );
        m_children.erase( iter );
        return result;
    }

    void Switch::set( Index index, std::unique_ptr< Element > child )
    {
        child->parent( *this );

        auto const iter = m_children.find( index );
        if( iter != std::end( m_children ) )
        {
            iter->second->destroy();
        }

        if( index == m_activeIndex )
        {
            m_activeChild = child.get();
        }

        m_children.insert_or_assign( index, std::move( child ) );

        if( index == m_activeIndex )
        {
            changed();
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
            m_activeChild = iter->second.get();
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

    void Switch::range( Range const& range )
    {
        Element::range( { { 0.0f, 0.0f }, range.extent() } );

        if( m_activeChild )
        {
            m_activeChild->range( range );
        }
    }

    Switch::Range Switch::fit( Range const& space )
    {
        if( m_activeChild )
        {
            return m_activeChild->fit( space );
        }

        return { space.min(), space.min() };
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

    Element* Switch::child( std::string const& id )
    {
        Index const index = std::stoi( id );
        auto const iter = m_children.find( index );
        return iter == std::end( m_children ) ? nullptr : iter->second.get();
    }

    Element const* Switch::child( std::string const& id ) const
    {
        Index const index = std::stoi( id );
        auto const iter = m_children.find( index );
        return iter == std::end( m_children ) ? nullptr : iter->second.get();
    }

    Switch::Switch( Framework& framework ) : Element{ framework }
    {
    }
}
