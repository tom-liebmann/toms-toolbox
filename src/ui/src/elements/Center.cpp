#include <ttb/ui/elements/Center.hpp>


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
            [ this ]( auto const& pos ) { return transform( pos ); },
            [ this ]( auto const& pos ) { return transformInv( pos ); } );
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
                        m_offset( 0 ) = ( size( 0 ) - childSize( 0 ) ) / 2.0f;
                        break;

                    case HAlignment::RIGHT:
                        m_offset( 0 ) = size( 0 ) - childSize( 0 );
                        break;
                }

                switch( m_vAlign )
                {
                    case VAlignment::TOP:
                        result( 1 ) = childSize( 1 );
                        break;

                    case VAlignment::MIDDLE:
                        m_offset( 1 ) = ( size( 1 ) - childSize( 1 ) ) / 2.0f;
                        break;

                    case VAlignment::BOTTOM:
                        m_offset( 0 ) = size( 1 ) - childSize( 1 );
                        break;
                }
            }

            return Element::fit( result );
        }

        return Element::fit( size );
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
