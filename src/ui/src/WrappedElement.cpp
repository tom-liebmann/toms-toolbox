#include <ttb/ui/WrappedElement.hpp>

#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>


namespace ttb::ui
{
    WrappedElement::WrappedElement( Framework& framework )
        : Element{ framework }, m_transform( ttb::mat::identity< float, 3 >() )
    {
    }

    void WrappedElement::destroy()
    {
        if( m_child )
        {
            m_child->destroy();
        }
    }

    void WrappedElement::range( Range const& range )
    {
        if( m_child )
        {
            auto const childRange = m_child->fit( range );
            m_child->range( { childRange.min() - range.min(), childRange.extent() } );
        }

        m_transform( 0, 2 ) = range.min( 0 );
        m_transform( 1, 2 ) = range.min( 1 );

        Element::range( range );
    }

    WrappedElement::Range WrappedElement::fit( Range const& space )
    {
        if( m_child )
        {
            return m_child->fit( space );
        }
        else
        {
            return space;
        }
    }

    void WrappedElement::update( float timeDiff )
    {
        if( m_child )
        {
            m_child->update( timeDiff );
        }
    }

    void WrappedElement::render( ttb::State& state ) const
    {
        if( m_child )
        {
            auto const u =
                state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ).push( m_transform );

            m_child->render( state );
        }
    }

    bool WrappedElement::onEvent( Event const& event )
    {
        if( m_child )
        {
            return m_child->onEvent( event );
        }

        return false;
    }

    void WrappedElement::wrappedChild( Element* child )
    {
        if( m_child )
        {
            m_child->destroy();
        }

        m_child = child;

        if( m_child )
        {
            m_child->parent( this );
        }

        changed();
    }

    Element* WrappedElement::wrappedChild()
    {
        return m_child;
    }
}
