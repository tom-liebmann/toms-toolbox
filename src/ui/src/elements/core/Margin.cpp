#include <ttb/ui/elements/core/Margin.hpp>


namespace ttb::ui
{
    void Margin::child( std::unique_ptr< Element > element )
    {
        wrappedChild( std::move( element ) );
    }

    void Margin::range( Range const& range )
    {
        ElementWrapper::range( range );

        if( !wrappedChild() )
        {
            return;
        }

        Range const subRange = { { m_left, m_top },
                                 { range.extent( 0 ) - m_right, range.extent( 1 ) - m_bottom } };

        wrappedChild()->range( subRange );
    }

    Element::Range Margin::fit( Range const& space )
    {
        if( !wrappedChild() )
        {
            return space;
        }

        Range const subSpace{ { m_left, m_top },
                              { space.extent( 0 ) - m_right, space.extent( 1 ) - m_bottom } };

        auto childSpace = wrappedChild()->fit( subSpace );

        return { { space.min( 0 ) + childSpace.min( 0 ) - m_left,
                   space.min( 1 ) + childSpace.min( 1 ) - m_top },
                 { space.min( 0 ) + childSpace.max( 0 ) + m_right,
                   space.min( 1 ) + childSpace.max( 1 ) + m_bottom } };
    }

    Margin::Margin( Framework& framework, float right, float top, float left, float bottom )
        : ElementWrapper{ framework }
        , m_right{ right }
        , m_top{ top }
        , m_left{ left }
        , m_bottom{ bottom }
    {
    }
}
