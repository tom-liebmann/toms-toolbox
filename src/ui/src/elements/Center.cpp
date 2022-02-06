#include <ttb/ui/elements/Center.hpp>

#include <ttb/core/uniform.hpp>
#include <ttb/math/matrix_operations.hpp>


namespace ttb::ui
{
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
