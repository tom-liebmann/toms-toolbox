#include <ttb/ui/elements/core/Center.hpp>

#include <cassert>


namespace ttb::ui
{
    void Center::child( std::unique_ptr< Element > element )
    {
        wrappedChild( std::move( element ) );
    }

    void Center::range( Range const& range )
    {
        ElementWrapper::range( range );

        auto childRange = wrappedChild()->fit( Range{ { 0.0f, 0.0f }, range.extent() } );

        switch( m_hAlign )
        {
            case HAlignment::LEFT:
            {
                auto const childWidth = childRange.extent( 0 );
                childRange.min( 0, 0.0f );
                childRange.max( 0, childWidth );
                break;
            }

            case HAlignment::CENTER:
            {
                auto const childWidth = childRange.extent( 0 );
                childRange.min( 0, ( range.extent( 0 ) - childWidth ) / 2.0f );
                childRange.max( 0, ( range.extent( 0 ) + childWidth ) / 2.0f );
                break;
            }

            case HAlignment::RIGHT:
            {
                auto const childWidth = childRange.extent( 0 );
                childRange.min( 0, range.extent( 0 ) - childWidth );
                childRange.max( 0, range.extent( 0 ) );
                break;
            }
        }

        switch( m_vAlign )
        {
            case VAlignment::TOP:
            {
                auto const childHeight = childRange.extent( 1 );
                childRange.min( 1, 0.0f );
                childRange.max( 1, childHeight );
                break;
            }

            case VAlignment::MIDDLE:
            {
                auto const childHeight = childRange.extent( 1 );
                childRange.min( 1, ( range.extent( 1 ) - childHeight ) / 2.0f );
                childRange.max( 1, ( range.extent( 1 ) + childHeight ) / 2.0f );
                break;
            }

            case VAlignment::BOTTOM:
            {
                auto const childHeight = childRange.extent( 1 );
                childRange.min( 1, range.extent( 1 ) - childHeight );
                childRange.max( 1, range.extent( 1 ) );
                break;
            }
        }

        wrappedChild()->range( childRange );
    }

    auto Center::fit( Range const& space ) -> Range
    {
        assert( wrappedChild() != nullptr );

        auto const childFit = wrappedChild()->fit( space );

        auto range = space;
        {
            switch( m_hAlign )
            {
                case HAlignment::LEFT:
                    range.extent( 0, childFit.extent( 0 ) );
                    break;

                case HAlignment::CENTER:
                case HAlignment::RIGHT:
                    break;
            }

            switch( m_vAlign )
            {
                case VAlignment::TOP:
                    range.extent( 1, childFit.extent( 1 ) );
                    break;

                case VAlignment::MIDDLE:
                case VAlignment::BOTTOM:
                    break;
            }
        }

        return range;
    }

    Center::Center( Framework& framework, HAlignment hAlign, VAlignment vAlign )
        : ElementWrapper{ framework }, m_hAlign{ hAlign }, m_vAlign{ vAlign }
    {
    }
}
