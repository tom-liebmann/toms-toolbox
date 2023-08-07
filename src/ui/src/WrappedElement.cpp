#include <ttb/ui/WrappedElement.hpp>

#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>


namespace ttb::ui
{
    WrappedElement::WrappedElement( Root& root ) : Element{ root }
    {
    }

    float WrappedElement::fitWidth( float space ) const
    {
        if( getWidth().getType() == Extent::Type::MATCH_CHILD )
        {
            return ( m_child ? m_child->fitWidth( space - getLeft() - getRight() ) : 0.0f ) +
                   getLeft() + getRight();
        }

        return Element::fitWidth( space );
    }

    float WrappedElement::fitHeight( float space ) const
    {
        if( getHeight().getType() == Extent::Type::MATCH_CHILD )
        {
            return ( m_child ? m_child->fitHeight( space - getTop() - getBottom() ) : 0.0f ) +
                   getTop() + getBottom();
        }

        return Element::fitHeight( space );
    }

    void WrappedElement::setPosition( Position const& value )
    {
        Element::setPosition( value );

        if( m_child )
        {
            m_child->setPosition( getPosition() );
        }
    }

    void WrappedElement::setSize( Size const& value )
    {
        Element::setSize( value );

        if( m_child )
        {
            m_child->setSize( getSize() );
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

    void WrappedElement::onChildChanged( Element& /* child */ )
    {
        changed();
    }

    void WrappedElement::wrappedChild( Element* child )
    {
        if( m_child )
        {
            m_child->setParent( nullptr );
        }

        m_child = child;

        if( m_child )
        {
            m_child->setParent( this );
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
