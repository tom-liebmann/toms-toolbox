#include <ttb/ui/CombinedElement.hpp>

#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>


namespace ttb::ui
{
    CombinedElement::CombinedElement( Framework& framework ) : Element{ framework }
    {
    }

    void CombinedElement::destroy()
    {
        for( auto const& child : m_children )
        {
            child.element->destroy();
        }
    }

    auto CombinedElement::fit( Size const& size ) -> Size
    {
        auto maxSize = Size{ 0.0f, 0.0f };

        for( auto const& child : m_children )
        {
            auto const childSize = child.element->fit( size );

            if( child.considerSize )
            {
                maxSize = max( maxSize, childSize );
            }
        }

        return Element::fit( maxSize );
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
        for( auto const& child : m_children )
        {
            if( child.element->onEvent( event ) )
            {
                return true;
            }
        }

        return false;
    }

    void CombinedElement::add( Element* child,
                               bool considerSize,
                               Transform transform,
                               Transform transformInv )
    {
        child->parent( this, std::move( transform ), std::move( transformInv ) );
        m_children.push_back( Slot{ considerSize, child } );
        changed();
    }
}
