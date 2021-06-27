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

    void Element::parent( Element* parent, Transform transform, Transform transformInv )
    {
        m_parent = parent;
        m_transform = std::move( transform );
        m_transformInv = std::move( transformInv );
    }

    auto Element::size() const -> Size const&
    {
        return m_size;
    }

    auto Element::fit( Size const& size ) -> Size
    {
        m_size = size;

        return m_size;
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

    ttb::Vector< float, 2 > Element::localToScreen( ttb::Vector< float, 2 > const& vec ) const
    {
        auto const v = m_transform ? m_transform( vec ) : vec;
        return m_parent ? m_parent->localToScreen( v ) : v;
    }

    ttb::Vector< float, 2 > Element::screenToLocal( ttb::Vector< float, 2 > const& vec ) const
    {
        auto const v = m_parent ? m_parent->screenToLocal( vec ) : vec;
        return m_transformInv ? m_transformInv( v ) : v;
    }

    void Element::onChildChanged( Element& /* child */ )
    {
        changed();
    }
}
