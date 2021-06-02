#include <ttb/ui/elements/core/Priority.hpp>


namespace ttb::ui
{
    std::unique_ptr< Element > Priority::releaseFirstChild()
    {
        if( m_newFirstChild )
        {
            m_newFirstChild->destroy();
            m_newFirstChild.reset();
        }

        m_firstChildChanged = true;
        return std::move( m_firstChild );
    }

    void Priority::firstChild( std::unique_ptr< Element > child )
    {
        m_newFirstChild = std::move( child );
        m_firstChildChanged = true;
    }

    void Priority::secondChild( std::unique_ptr< Element > child )
    {
        m_newSecondChild = std::move( child );
        m_secondChildChanged = true;
    }

    void Priority::destroy()
    {
        if( m_newFirstChild )
        {
            m_newFirstChild->destroy();
        }

        if( m_newSecondChild )
        {
            m_newSecondChild->destroy();
        }

        if( m_firstChild )
        {
            m_firstChild->destroy();
        }

        if( m_secondChild )
        {
            m_secondChild->destroy();
        }
    }

    void Priority::range( Range const& range )
    {
        if( m_firstChild )
        {
            return m_firstChild->range( range );
        }

        if( m_secondChild )
        {
            return m_secondChild->range( range );
        }
    }

    Priority::Range Priority::fit( Range const& space )
    {
        if( m_firstChild )
        {
            return m_firstChild->fit( space );
        }

        if( m_secondChild )
        {
            return m_secondChild->fit( space );
        }

        return space;
    }

    void Priority::update( float timeDiff )
    {
        if( m_firstChildChanged )
        {
            if( m_firstChild )
            {
                m_firstChild->destroy();
            }

            m_firstChild = std::move( m_newFirstChild );
            m_firstChildChanged = false;

            if( m_firstChild )
            {
                m_firstChild->parent( *this );
            }

            changed();
        }

        if( m_secondChildChanged )
        {
            if( m_secondChild )
            {
                m_secondChild->destroy();
            }

            m_secondChild = std::move( m_newSecondChild );
            m_secondChildChanged = false;

            if( m_secondChild )
            {
                m_secondChild->parent( *this );
            }

            if( !m_firstChild )
            {
                changed();
            }
        }

        if( m_firstChild )
        {
            return m_firstChild->update( timeDiff );
        }

        if( m_secondChild )
        {
            return m_secondChild->update( timeDiff );
        }
    }

    void Priority::render( ttb::State& state ) const
    {
        if( m_firstChild )
        {
            return m_firstChild->render( state );
        }

        if( m_secondChild )
        {
            return m_secondChild->render( state );
        }
    }

    bool Priority::onEvent( Event const& event )
    {
        if( m_firstChild )
        {
            return m_firstChild->onEvent( event );
        }

        if( m_secondChild )
        {
            return m_secondChild->onEvent( event );
        }

        return false;
    }

    void Priority::onChildChanged( Element& child )
    {
        if( m_firstChild.get() == &child || !m_firstChild )
        {
            changed();
        }
    }

    Element* Priority::child( std::string const& id )
    {
        if( id == "0" )
        {
            return m_firstChild.get();
        }
        else if( id == "1" )
        {
            return m_secondChild.get();
        }

        return nullptr;
    }

    Element const* Priority::child( std::string const& id ) const
    {
        if( id == "0" )
        {
            return m_firstChild.get();
        }
        else if( id == "1" )
        {
            return m_secondChild.get();
        }

        return nullptr;
    }

    Priority::Priority( Framework& framework ) : Element{ framework }
    {
    }
}
