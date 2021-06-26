#include <ttb/ui/elements/Margin.hpp>


namespace ttb::ui
{
    Margin::Margin( Framework& framework, float right, float top, float left, float bottom )
        : WrappedElement{ framework }
        , m_right{ right }
        , m_top{ top }
        , m_left{ left }
        , m_bottom{ bottom }
    {
    }

    void Margin::child( Element* element )
    {
        wrappedChild( element );
    }

    void Margin::range( Range const& range )
    {
        WrappedElement::range( range );

        if( auto const child = wrappedChild(); child )
        {
            auto const subRange =
                Range{ { m_left, m_top },
                       { range.extent( 0 ) - m_right, range.extent( 1 ) - m_bottom } };

            child->range( subRange );
        }
    }

    Element::Range Margin::fit( Range const& space )
    {
        if( auto const child = wrappedChild(); child )
        {
            auto const subSpace =
                Range{ { m_left, m_top },
                       { space.extent( 0 ) - m_right, space.extent( 1 ) - m_bottom } };

            auto childSpace = child->fit( subSpace );

            return { { space.min( 0 ) + childSpace.min( 0 ) - m_left,
                       space.min( 1 ) + childSpace.min( 1 ) - m_top },
                     { space.min( 0 ) + childSpace.max( 0 ) + m_right,
                       space.min( 1 ) + childSpace.max( 1 ) + m_bottom } };
        }

        return space;
    }
}
