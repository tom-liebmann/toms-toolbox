#include <ttb/ui/Element.hpp>

#include <ttb/math/vector_operations.hpp>

#include <ttb/ui/elements/Center.hpp>

#include "rapidxml.h"

#include <iostream>
#include <map>


namespace ttb::ui
{
    Element::Element( Root& root ) : m_root{ root }
    {
    }

    Element::~Element() = default;

    auto Element::fit( Size const& size ) -> Size
    {
        return size;
    }

    void Element::offset( Offset const& value )
    {
        m_offset = value;
    }

    void Element::size( Size const& value )
    {
        m_size = value;
    }

    void Element::update( float /* timeDiff */ )
    {
    }

    bool Element::onEvent( Event const& /* event */ )
    {
        return false;
    }

    std::string Element::info() const
    {
        return "Element";
    }

    void Element::changed()
    {
        if( m_parent )
        {
            m_parent->onChildChanged( *this );
        }
    }

    auto Element::transform() const -> Transform
    {
        // clang-format off
        return Transform{
            m_size( 0 ), 0.0f       , m_offset( 0 ),
            0.0f       , m_size( 1 ), m_offset( 1 ),
            0.0f       , 0.0f       , 1.0f
        };
        // clang-format on
    }
}
