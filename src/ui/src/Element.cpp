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
    }
}


namespace ttb::ui
{
    Element::Element( Root& root ) : m_root{ root }
    {
    }

    Element::~Element() = default;

    FitExtent Element::fitWidth( Size const& /* space */ ) const
    {
        switch( m_width.getType() )
        {
            case Extent::Type::MATCH_PARENT:
                return { FitExtent::Type::MATCH_PARENT };
            case Extent::Type::FIXED:
                return { m_width.getValue() + m_margin.getRightLeft() };
            case Extent::Type::MATCH_CHILD:
                throw std::runtime_error( "Invalid child match for non-nested element" );
            case Extent::Type::UNSPECIFIED:
            default:
                throw std::runtime_error( "Element width unspecified" );
        }
    }

    FitExtent Element::fitHeight( Size const& /* space */ ) const
    {
        switch( m_height.getType() )
        {
            case Extent::Type::MATCH_PARENT:
                return { FitExtent::Type::MATCH_PARENT };
            case Extent::Type::FIXED:
                return { m_height.getValue() + m_margin.getTopBottom() };
            case Extent::Type::MATCH_CHILD:
                throw std::runtime_error( "Invalid child match for non-nested element" );
            case Extent::Type::UNSPECIFIED:
            default:
                throw std::runtime_error( "Element width unspecified" );
        }
    }

    auto Element::finalFit( Size const& space ) const -> Size
    {
        auto const extentH = fitWidth( space );
        auto const extentV = fitHeight( space );

        return {
            extentH.getType() == FitExtent::Type::MATCH_PARENT ? space( 0 ) : extentH.getValue(),
            extentV.getType() == FitExtent::Type::MATCH_PARENT ? space( 1 ) : extentV.getValue()
        };
    }

    void Element::setPosition( Position const& value )
    {
        m_position( 0 ) = value( 0 ) + m_margin.getLeft();
        m_position( 1 ) = value( 1 ) + m_margin.getTop();
    }

    void Element::setSize( Size const& value )
    {
        m_size( 0 ) = value( 0 ) - m_margin.getRightLeft();
        m_size( 1 ) = value( 1 ) - m_margin.getTopBottom();
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

    auto Element::getTransform() const -> Transform
    {
        // clang-format off
        return Transform{
            m_size( 0 ), 0.0f       , m_position( 0 ),
            0.0f       , m_size( 1 ), m_position( 1 ),
            0.0f       , 0.0f       , 1.0f
        };
        // clang-format on
    }

    void Element::parseXml( XmlNode const& node, XmlLoader& loader )
    {
        m_margin = Margin::parse( node, loader );

        if( auto const value = loader.attrValue( node, "width" ) )
        {
            m_width = parseExtent( *value );
        }

        if( auto const value = loader.attrValue( node, "height" ) )
        {
            m_height = parseExtent( *value );
        }
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
    }
}
