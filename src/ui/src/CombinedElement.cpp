#include <ttb/ui/CombinedElement.hpp>

#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>

#include <numeric>


namespace ttb::ui
{
    CombinedElement::CombinedElement( Root& root ) : Element{ root }
    {
    }

    auto CombinedElement::fit( Size const& size ) -> Size
    {
        return std::accumulate( std::begin( m_children ),
                                std::end( m_children ),
                                Size{},
                                [ &size ]( auto const& maxSize, auto const& child )
                                {
                                    if( child.considerSize )
                                    {
                                        return max( maxSize, child.element->fit( size ) );
                                    }

                                    return maxSize;
                                } );
    }

    void CombinedElement::size( Size const& value )
    {
        Element::size( value );

        for( auto const& child : m_children )
        {
            child.element->size( value );
        }
    }

    void CombinedElement::offset( Offset const& value )
    {
        Element::offset( value );

        for( auto const& child : m_children )
        {
            child.element->offset( value );
        }
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
        for( auto const& child : m_children )
        {
            child.element->render( state );
        }
    }

    bool CombinedElement::onEvent( Event const& event )
    {
        return std::any_of( std::rbegin( m_children ),
                            std::rend( m_children ),
                            [ &event ]( auto const& child )
                            {
                                return child.element->onEvent( event );
                            } );
    }

    void CombinedElement::onChildChanged( Element& /* child */ )
    {
        changed();
    }

    void CombinedElement::remove( Element& child )
    {
        auto const newEnd = std::remove_if( std::begin( m_children ),
                                            std::end( m_children ),
                                            [ &child ]( auto const& slot )
                                            {
                                                return slot.element == &child;
                                            } );

        if( newEnd == std::end( m_children ) )
        {
            return;
        }

        child.setParent( nullptr );

        m_children.erase( newEnd, std::end( m_children ) );
        changed();
    }

    void CombinedElement::insert( std::size_t position, Element* child, bool considerSize )
    {
        child->setParent( this );
        m_children.insert( std::next( std::begin( m_children ), position ),
                           Slot{ considerSize, child } );
        changed();
    }

    void CombinedElement::add( Element* child, bool considerSize )
    {
        child->setParent( this );
        m_children.push_back( Slot{ considerSize, child } );
        changed();
    }
}
