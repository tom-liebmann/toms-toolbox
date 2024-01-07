#include <ttb/ui/elements/Align.hpp>

#include <ttb/core/uniform.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>
#include <ttb/ui/XmlFactory.hpp>


namespace ttb::ui
{
    namespace
    {
        auto const factory = XmlFactory< Align >{ "align" };

        Align::HAlignment parseHAlignment( std::string_view const& value );

        Align::VAlignment parseVAlignment( std::string_view const& value );
    }
}


namespace ttb::ui
{
    Align::Align( Root& root ) : Align{ root, HAlignment::CENTER, VAlignment::MIDDLE }
    {
    }

    Align::Align( Root& root, HAlignment hAlign, VAlignment vAlign )
        : Slot{ root }, m_hAlign{ hAlign }, m_vAlign{ vAlign }
    {
        setWidth( Extent::Type::MATCH_CHILD );
        setHeight( Extent::Type::MATCH_CHILD );
    }

    void Align::parseXml( XmlNode const& node, XmlLoader& loader )
    {
        if( auto const value = loader.attrValue( node, "h" ) )
        {
            m_hAlign = parseHAlignment( *value );
        }

        if( auto const value = loader.attrValue( node, "v" ) )
        {
            m_vAlign = parseVAlignment( *value );
        }

        Slot::parseXml( node, loader );
    }

    FitExtent Align::fitWidth( Size const& space ) const
    {
        if( m_hAlign == HAlignment::LEFT )
        {
            if( auto const child = getChild() )
            {
                auto const margin = getMargin();
                auto const childFit = child->fitWidth(
                    { space( 0 ) - margin.getRightLeft(), space( 1 ) - margin.getTopBottom() } );
                return childFit.getType() == FitExtent::Type::FIXED
                           ? childFit.getValue() + margin.getRightLeft()
                           : childFit;
            }
        }

        return FitExtent::Type::MATCH_PARENT;
    }

    FitExtent Align::fitHeight( Size const& space ) const
    {
        if( m_vAlign == VAlignment::TOP )
        {
            if( auto const child = getChild() )
            {
                auto const margin = getMargin();
                auto const childFit = child->fitHeight(
                    { space( 0 ) - margin.getRightLeft(), space( 1 ) - margin.getTopBottom() } );
                return childFit.getType() == FitExtent::Type::FIXED
                           ? childFit.getValue() + margin.getTopBottom()
                           : childFit;
            }
        }

        return FitExtent::Type::MATCH_PARENT;
    }

    void Align::setSize( Size const& value )
    {
        Element::setSize( value );

        auto const size = getSize();

        if( auto const child = getChild() )
        {
            auto const childSize = child->finalFit( size );

            switch( m_hAlign )
            {
                case HAlignment::LEFT:
                    m_childOffset( 0 ) = 0.0f;
                    break;

                case HAlignment::CENTER:
                    m_childOffset( 0 ) = ( size( 0 ) - childSize( 0 ) ) / 2.0f;
                    break;

                case HAlignment::RIGHT:
                    m_childOffset( 0 ) = size( 0 ) - childSize( 0 );
                    break;
            }

            switch( m_vAlign )
            {
                case VAlignment::TOP:
                    m_childOffset( 1 ) = 0.0f;
                    break;

                case VAlignment::MIDDLE:
                    m_childOffset( 1 ) = ( size( 1 ) - childSize( 1 ) ) / 2.0f;
                    break;

                case VAlignment::BOTTOM:
                    m_childOffset( 1 ) = size( 1 ) - childSize( 1 );
                    break;
            }

            // The child always gets the size it wants. Align just aligns it accordingly using the
            // offset.
            child->setSize( childSize );
        }
    }

    void Align::setPosition( Position const& value )
    {
        Element::setPosition( value );

        if( auto const child = getChild() )
        {
            child->setPosition( getPosition() + m_childOffset );
        }
    }
}


namespace ttb::ui
{
    namespace
    {
        inline Align::HAlignment parseHAlignment( std::string_view const& value )
        {
            if( value == "left" )
            {
                return Align::HAlignment::LEFT;
            }

            if( value == "center" )
            {
                return Align::HAlignment::CENTER;
            }

            if( value == "right" )
            {
                return Align::HAlignment::RIGHT;
            }

            throw std::runtime_error( "Invalid alignment value" );
        }

        inline Align::VAlignment parseVAlignment( std::string_view const& value )
        {
            if( value == "top" )
            {
                return Align::VAlignment::TOP;
            }

            if( value == "middle" )
            {
                return Align::VAlignment::MIDDLE;
            }

            if( value == "bottom" )
            {
                return Align::VAlignment::BOTTOM;
            }

            throw std::runtime_error( "Invalid alignment value" );
        }
    }
}
