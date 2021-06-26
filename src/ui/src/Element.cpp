#include <ttb/ui/Element.hpp>

#include <ttb/math/vector_operations.hpp>


namespace ttb::ui
{
    Element::Element( Framework& framework ) : m_framework{ framework }
    {
    }

    Element::~Element() = default;

    void Element::destroy()
    {
    }

    Element* Element::parent() const
    {
        return m_parent;
    }

    void Element::parent( Element* parent )
    {
        m_parent = parent;
    }

    Element::Range const& Element::range() const
    {
        return m_range;
    }

    void Element::range( Element::Range const& range )
    {
        m_range = range;
    }

    Element::Range Element::fit( Range const& space )
    {
        return space;
    }

    void Element::update( float /* timeDiff */ )
    {
    }

    bool Element::onEvent( Event const& /* event */ )
    {
        return false;
    }

    void Element::changed()
    {
        if( m_parent )
        {
            m_parent->onChildChanged( *this );
        }
    }

    ttb::Vector< float, 2 > Element::screenToParent( ttb::Vector< float, 2 > const& vec ) const
    {
        return m_parent ? m_parent->screenToLocal( vec ) : vec;
    }

    ttb::Vector< float, 2 > Element::screenToLocal( ttb::Vector< float, 2 > const& vec ) const
    {
        return screenToParent( vec ) - m_range.min();
    }

    ttb::Vector< float, 2 > Element::localToScreen( ttb::Vector< float, 2 > const& vec ) const
    {
        return ( m_parent ? m_parent->localToScreen( vec ) : vec ) + m_range.min();
    }

    void Element::onChildChanged( Element& /* child */ )
    {
        changed();
    }
}
