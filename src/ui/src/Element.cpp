#include <ttb/ui/Element.hpp>

#include <ttb/math/vector_operations.hpp>

#include "rapidxml.h"

#include <iostream>
#include <map>


namespace ttb::ui
{
    Element::Element( Root& root ) : m_root{ root }
    {
    }

    Element::Element( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader )
    {
        if( auto const value = loader.attrValue( node, "width" ) )
        {
            m_width = parseExtent( value );
        }

        if( auto const value = loader.attrValue( node, "height" ) )
        {
            m_height = parseExtent( value );
        }

        if( auto const value = loader.attrValue( node, "right" ) )
        {
            m_right = parseOffset( value );
        }

        if( auto const value = loader.attrValue( node, "top" ) )
        {
            m_top = parseOffset( value );
        }

        if( auto const value = loader.attrValue( node, "left" ) )
        {
            m_left = parseOffset( value );
        }

        if( auto const value = loader.attrValue( node, "bottom" ) )
        {
            m_bottom = parseOffset( value );
        }
    }

    Element::~Element() = default;

    auto Element::fit( Size const& size ) -> Size
    {
        return size;
    }

    void Element::setPosition( Position const& value )
    {
        m_position = value;
    }

    void Element::setSize( Size const& value )
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
            m_size( 0 ), 0.0f       , m_position( 0 ),
            0.0f       , m_size( 1 ), m_position( 1 ),
            0.0f       , 0.0f       , 1.0f
        };
        // clang-format on
    }
}
