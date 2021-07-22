#include <ttb/ui/WrappedElement.hpp>

#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>


namespace ttb::ui
{
    WrappedElement::WrappedElement( Framework& framework ) : Element{ framework }
    {
    }

    void WrappedElement::destroy()
    {
        if( m_child )
        {
            m_child->destroy();
        }
    }

    auto WrappedElement::fit( Size const& size ) -> Size
    {
        return Element::fit( m_child ? m_child->fit( size ) : Size{ 0.0f, 0.0f } );
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

    void WrappedElement::wrappedChild( Element* child, Transform transform, Transform transformInv )
    {
        if( m_child )
        {
            m_child->destroy();
        }

        m_child = child;

        if( m_child )
        {
            m_child->parent( this, std::move( transform ), std::move( transformInv ) );
        }

        changed();
    }

    Element* WrappedElement::wrappedChild()
    {
        return m_child;
    }

    Element const* WrappedElement::wrappedChild() const
    {
        return m_child;
    }
}
