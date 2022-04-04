#include <ttb/ui/elements/Center.hpp>

#include <ttb/core/uniform.hpp>
#include <ttb/math/matrix_operations.hpp>
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
    Center::Center( Framework& framework, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : WrappedElement{ framework }
        , m_hAlign{ HAlignment::CENTER }
        , m_vAlign{ VAlignment::MIDDLE }
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
            wrappedChild( loader.loadElement( framework, *child ) );
        }
    }

    Center::Center( Framework& framework, HAlignment hAlign, VAlignment vAlign )
        : WrappedElement{ framework }, m_hAlign{ hAlign }, m_vAlign{ vAlign }
    {
    }

    void Center::child( Element* element )
    {
        wrappedChild(
            element,
            [ this ]( auto const& pos )
            {
                return transform( pos );
            },
            [ this ]( auto const& pos )
            {
                return transformInv( pos );
            } );
    }

    auto Center::fit( Size const& size ) -> Size
    {
        if( auto const child = wrappedChild(); child )
        {
            auto const childSize = child->fit( size );

            auto result = size;
            m_offset = { 0.0f, 0.0f };
            {
                switch( m_hAlign )
                {
                    case HAlignment::LEFT:
                        result( 0 ) = childSize( 0 );
                        break;

                    case HAlignment::CENTER:
                        result( 0 ) = ( size( 0 ) + childSize( 0 ) ) / 2.0f;
                        m_offset( 0 ) = ( size( 0 ) - childSize( 0 ) ) / 2.0f;
                        break;

                    case HAlignment::RIGHT:
                        result( 0 ) = size( 0 );
                        m_offset( 0 ) = size( 0 ) - childSize( 0 );
                        break;
                }

                switch( m_vAlign )
                {
                    case VAlignment::TOP:
                        result( 1 ) = childSize( 1 );
                        break;

                    case VAlignment::MIDDLE:
                        result( 1 ) = ( size( 1 ) + childSize( 1 ) ) / 2.0f;
                        m_offset( 1 ) = ( size( 1 ) - childSize( 1 ) ) / 2.0f;
                        break;

                    case VAlignment::BOTTOM:
                        result( 1 ) = size( 1 );
                        m_offset( 1 ) = size( 1 ) - childSize( 1 );
                        break;
                }
            }

            if( childSize( 0 ) > result( 0 ) )
            {
                result( 0 ) = childSize( 0 );
                m_offset( 0 ) = 0.0f;
            }

            if( childSize( 1 ) > result( 1 ) )
            {
                result( 1 ) = childSize( 1 );
                m_offset( 1 ) = 0.0f;
            }

            return Element::fit( result );
        }

        return Element::fit( size );
    }

    void Center::render( ttb::State& state ) const
    {
        if( auto const child = wrappedChild(); child )
        {
            state.with( ttb::UniformBinder( "u_transform", ttb::mat::translation( m_offset ) ),
                        [ & ]
                        {
                            child->render( state );  //
                        } );
        }
    }

    std::string Center::info() const
    {
        return "Center";
    }

    bool Center::onEvent( ttb::Event const& event )
    {
        auto result = false;

        if( auto const child = wrappedChild(); child )
        {
            event.transform(
                [ this ]( auto const& v )
                {
                    return v - m_offset;
                },
                [ &child, &result ]( auto const& event )
                {
                    result = child->onEvent( event );
                } );
        }

        return result;
    }

    auto Center::transform( Position const& pos ) const -> Position
    {
        return pos + m_offset;
    }

    auto Center::transformInv( Position const& pos ) const -> Position
    {
        return pos - m_offset;
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
