#include <ttb/ui/CombinedElement.hpp>

#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>


namespace ttb::ui
{
    CombinedElement::CombinedElement( Framework& framework )
        : Element{ framework }, m_transform( ttb::mat::identity< float, 3 >() )
    {
    }

    void CombinedElement::destroy()
    {
        for( auto const& child : m_children )
        {
            child.element->destroy();
        }
    }

    void CombinedElement::range( Range const& range )
    {
        for( auto const& child : m_children )
        {
            auto const childRange = child.element->fit( range );
            child.element->range( { childRange.min() - range.min(), childRange.extent() } );
        }

        m_transform( 0, 2 ) = range.min( 0 );
        m_transform( 1, 2 ) = range.min( 1 );

        Element::range( range );
    }

    CombinedElement::Range CombinedElement::fit( Range const& space )
    {
        auto bound = Range{
            { std::numeric_limits< float >::infinity(), std::numeric_limits< float >::infinity() },
            { -std::numeric_limits< float >::infinity(), -std::numeric_limits< float >::infinity() }
        };

        for( auto const& child : m_children )
        {
            if( child.considerSize )
            {
                bound.extend( child.element->fit( space ) );
            }
        }

        return bound;
    }

    void CombinedElement::update( float timeDiff )
    {
        for( auto const& child : m_children )
        {
            child.element->update( timeDiff );
        }
    }

    void CombinedElement::render( ttb::State& state ) const
    {
        auto const u =
            state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ).push( m_transform );

        for( auto const& child : m_children )
        {
            child.element->render( state );
        }
    }

    bool CombinedElement::onEvent( Event const& event )
    {
        for( auto const& child : m_children )
        {
            if( child.element->onEvent( event ) )
            {
                return true;
            }
        }

        return false;
    }

    void CombinedElement::add( Element* child, bool considerSize )
    {
        child->parent( this );
        m_children.push_back( Slot{ considerSize, child } );
        changed();
    }
}
