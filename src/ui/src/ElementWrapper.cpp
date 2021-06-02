#include <ttb/ui/ElementWrapper.hpp>


namespace ttb::ui
{
    ElementWrapper::ElementWrapper( Framework& framework )
        : Element{ framework }, m_transform( ttb::mat::identity< float, 3 >() )
    {
    }

    ElementWrapper::~ElementWrapper()
    {
    }

    void ElementWrapper::range( Range const& range )
    {
        Element::range( range );
        m_transform( 0, 2 ) = range.min( 0 );
        m_transform( 1, 2 ) = range.min( 1 );

        if( m_child )
        {
            auto const childRange = Range{ { 0.0f, 0.0f }, range.extent() };
            m_child->range( childRange );
        }
    }

    void ElementWrapper::destroy()
    {
        if( m_child )
        {
            m_child->destroy();
        }
    }

    ElementWrapper::Range ElementWrapper::fit( Range const& space )
    {
        return m_child ? m_child->fit( space ) : space;
    }

    void ElementWrapper::update( float timeDiff )
    {
        if( m_child )
        {
            m_child->update( timeDiff );
        }
    }

    void ElementWrapper::render( ttb::State& state ) const
    {
        if( m_child )
        {
            auto const u =
                state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ).push( m_transform );

            m_child->render( state );
        }
    }

    bool ElementWrapper::onEvent( Event& event )
    {
        return m_child ? m_child->onEvent( event ) : false;
    }

    Element* ElementWrapper::wrappedChild()
    {
        return m_child.get();
    }

    Element const* ElementWrapper::wrappedChild() const
    {
        return m_child.get();
    }

    void ElementWrapper::wrappedChild( std::unique_ptr< Element > element )
    {
        if( m_child )
        {
            m_child->destroy();
        }

        m_child = std::move( element );

        if( m_child )
        {
            m_child->parent( *this );
        }

        changed();
    }
}
