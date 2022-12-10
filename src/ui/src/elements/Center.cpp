#include <ttb/ui/elements/Center.hpp>

#include <ttb/core/uniform.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>
#include <ttb/ui/XmlFactory.hpp>


namespace ttb::ui
{
    namespace
    {
        auto const factory = XmlFactory< Center >{ "center" };

        Center::HAlignment parseHAlignment( std::string_view const& value );

        Center::VAlignment parseVAlignment( std::string_view const& value );
    }
}


namespace ttb::ui
{
    Center::Center( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : WrappedElement{ root }, m_hAlign{ HAlignment::CENTER }, m_vAlign{ VAlignment::MIDDLE }
    {
        if( auto const value = loader.attrValue( node, "h" ); value )
        {
            m_hAlign = parseHAlignment( *value );
        }

        if( auto const value = loader.attrValue( node, "v" ); value )
        {
            m_vAlign = parseVAlignment( *value );
        }

        if( auto const child = node.first_node(); child )
        {
            wrappedChild( loader.loadElement( root, *child ) );
        }
    }

    Center::Center( Root& root, HAlignment hAlign, VAlignment vAlign )
        : WrappedElement{ root }, m_hAlign{ hAlign }, m_vAlign{ vAlign }
    {
    }

    void Center::child( Element* element )
    {
        wrappedChild( element );
    }

    auto Center::fit( Size const& size ) -> Size
    {
        if( auto const child = wrappedChild(); child )
        {
            auto const childSize = child->fit( size );

            auto result = Size{};

            switch( m_hAlign )
            {
                case HAlignment::LEFT:
                    result( 0 ) = childSize( 0 );
                    break;

                case HAlignment::CENTER:
                case HAlignment::RIGHT:
                    result( 0 ) = size( 0 );
                    break;
            }

            switch( m_vAlign )
            {
                case VAlignment::TOP:
                    result( 1 ) = childSize( 1 );
                    break;

                case VAlignment::MIDDLE:
                case VAlignment::BOTTOM:
                    result( 1 ) = size( 1 );
                    break;
            }

            return min( result, size );
        }

        return size;
    }

    void Center::offset( Offset const& value )
    {
        Element::offset( value );

        if( auto const child = wrappedChild(); child )
        {
            child->offset( value + m_childOffset );
        }
    }

    void Center::size( Size const& value )
    {
        Element::size( value );

        if( auto const child = wrappedChild(); child )
        {
            auto const childSize = child->fit( value );

            switch( m_hAlign )
            {
                case HAlignment::LEFT:
                    m_childOffset( 0 ) = 0.0f;
                    break;

                case HAlignment::CENTER:
                    m_childOffset( 0 ) = ( value( 0 ) - childSize( 0 ) ) / 2.0f;
                    break;

                case HAlignment::RIGHT:
                    m_childOffset( 0 ) = value( 0 ) - childSize( 0 );
                    break;
            }

            switch( m_vAlign )
            {
                case VAlignment::TOP:
                    m_childOffset( 1 ) = 0.0f;
                    break;

                case VAlignment::MIDDLE:
                    m_childOffset( 1 ) = ( value( 1 ) - childSize( 1 ) ) / 2.0f;
                    break;

                case VAlignment::BOTTOM:
                    m_childOffset( 1 ) = value( 1 ) - childSize( 1 );
                    break;
            }

            // The child always gets the size it wants. Center just aligns it accordingly using the
            // offset.
            child->size( childSize );
        }
    }

    void Center::render( ttb::State& state ) const
    {
        if( auto const child = wrappedChild(); child )
        {
            child->render( state );
        }
    }

    std::string Center::info() const
    {
        return "Center";
    }
}


namespace ttb::ui
{
    namespace
    {
        inline Center::HAlignment parseHAlignment( std::string_view const& value )
        {
            if( value == "left" )
            {
                return Center::HAlignment::LEFT;
            }

            if( value == "center" )
            {
                return Center::HAlignment::CENTER;
            }

            if( value == "right" )
            {
                return Center::HAlignment::RIGHT;
            }

            throw std::runtime_error( "Invalid alignment value" );
        }

        inline Center::VAlignment parseVAlignment( std::string_view const& value )
        {
            if( value == "top" )
            {
                return Center::VAlignment::TOP;
            }

            if( value == "middle" )
            {
                return Center::VAlignment::MIDDLE;
            }

            if( value == "bottom" )
            {
                return Center::VAlignment::BOTTOM;
            }

            throw std::runtime_error( "Invalid alignment value" );
        }
    }
}
