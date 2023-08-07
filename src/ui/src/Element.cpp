#include <ttb/ui/Element.hpp>

#include <ttb/math/vector_operations.hpp>
#include <ttb/ui/XmlLoader.hpp>

#include "rapidxml.h"

#include <iostream>
#include <map>


namespace ttb::ui
{
    namespace
    {
        Extent parseExtent( std::string_view value );

        float parseOffset( std::string_view value );
    }
}


namespace ttb::ui
{
    Element::Element( Root& root ) : m_root{ root }
    {
    }

    Element::Element( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : m_root{ root }
    {
        if( auto const value = loader.attrValue( node, "width" ) )
        {
            m_width = parseExtent( *value );
        }

        if( auto const value = loader.attrValue( node, "height" ) )
        {
            m_height = parseExtent( *value );
        }

        if( auto const value = loader.attrValue( node, "right" ) )
        {
            m_right = parseOffset( *value );
        }

        if( auto const value = loader.attrValue( node, "top" ) )
        {
            m_top = parseOffset( *value );
        }

        if( auto const value = loader.attrValue( node, "left" ) )
        {
            m_left = parseOffset( *value );
        }

        if( auto const value = loader.attrValue( node, "bottom" ) )
        {
            m_bottom = parseOffset( *value );
        }
    }

    Element::~Element() = default;

    float Element::fitWidth( float space ) const
    {
        switch( m_width.getType() )
        {
            case Extent::Type::MATCH_PARENT:
                return space;

            case Extent::Type::FIXED:
                return m_width.getValue() + m_left + m_right;

            case Extent::Type::MATCH_CHILD:
                throw std::runtime_error( "Invalid child match for non-nested element" );

            case Extent::Type::UNSPECIFIED:
                throw std::runtime_error( "Element width unspecified" );
        }
    }

    float Element::fitHeight( float space ) const
    {
        switch( m_height.getType() )
        {
            case Extent::Type::MATCH_PARENT:
                return space;

            case Extent::Type::FIXED:
                return m_height.getValue() + m_top + m_bottom;

            case Extent::Type::MATCH_CHILD:
                throw std::runtime_error( "Invalid child match for non-nested element" );

            case Extent::Type::UNSPECIFIED:
                throw std::runtime_error( "Element height unspecified" );
        }
    }

    auto Element::fit( Size const& space ) const -> Size
    {
        return { fitWidth( space( 0 ) ), fitHeight( space( 1 ) ) };
    }

    void Element::setPosition( Position const& value )
    {
        m_position( 0 ) = value( 0 ) + m_left;
        m_position( 1 ) = value( 1 ) + m_top;
    }

    void Element::setSize( Size const& value )
    {
        m_size( 0 ) = value( 0 ) - m_left - m_right;
        m_size( 1 ) = value( 1 ) - m_top - m_bottom;
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


namespace ttb::ui
{
    namespace
    {
        Extent parseExtent( std::string_view value )
        {
            if( value == "parent" )
            {
                return { Extent::Type::MATCH_PARENT };
            }

            if( value == "child" )
            {
                return { Extent::Type::MATCH_CHILD };
            }

            return { fromStringView< float >::convert( value ) };
        }

        float parseOffset( std::string_view value )
        {
            return fromStringView< float >::convert( value );
        }
    }
}
