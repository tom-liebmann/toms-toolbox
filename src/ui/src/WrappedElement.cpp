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
        if( m_child )
        {
            return m_child->fit( size );
        }

        return size;
    }

    void WrappedElement::offset( Offset const& value )
    {
        Element::offset( value );

        if( m_child )
        {
            m_child->offset( value );
        }
    }

    void WrappedElement::size( Size const& value )
    {
        Element::size( value );

        if( m_child )
        {
            m_child->size( value );
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

    Element const* WrappedElement::wrappedChild() const
    {
        return m_child;
    }
}
