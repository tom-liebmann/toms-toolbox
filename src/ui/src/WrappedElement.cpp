#include <ttb/ui/WrappedElement.hpp>

#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>
#include <ttb/ui/exceptions/VanishingElementException.hpp>


namespace ttb::ui
{
    WrappedElement::WrappedElement( Root& root ) : Element{ root }
    {
    }

    FitExtent WrappedElement::fitWidth( Size const& space ) const
    {
        if( getWidth().getType() != Extent::Type::MATCH_CHILD )
        {
            return Element::fitWidth( space );
        }

        auto const margin = getMargin();

        auto const childFit = m_child->fitWidth(
            { space( 0 ) - margin.getRightLeft(), space( 1 ) - margin.getTopBottom() } );

        switch( childFit.getType() )
        {
            case FitExtent::Type::MATCH_PARENT:
                return childFit;
            case FitExtent::Type::FIXED:
                return { childFit.getValue() + margin.getRightLeft() };
            default:
                throw std::runtime_error( "Child extent unsupported" );
        }
    }

    FitExtent WrappedElement::fitHeight( Size const& space ) const
    {
        if( getHeight().getType() != Extent::Type::MATCH_CHILD )
        {
            return Element::fitHeight( space );
        }

        auto const margin = getMargin();

        auto const childFit = m_child->fitHeight(
            { space( 0 ) - margin.getRightLeft(), space( 1 ) - margin.getTopBottom() } );

        switch( childFit.getType() )
        {
            case FitExtent::Type::MATCH_PARENT:
                return childFit;
            case FitExtent::Type::FIXED:
                return { childFit.getValue() + margin.getTopBottom() };
            default:
                throw std::runtime_error( "Child extent unsupported" );
        }
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
