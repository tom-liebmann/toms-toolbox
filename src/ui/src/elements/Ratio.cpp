#include <ttb/ui/elements/Ratio.hpp>

#include <iostream>


namespace ttb::ui
{
    Ratio::Ratio( Framework& framework, float ratio )
        : WrappedElement{ framework }, m_ratio{ ratio }
    {
    }

    void Ratio::child( Element* element )
    {
        wrappedChild( element );
    }

    auto Ratio::fit( Range const& space ) -> Range
    {
        if( auto const child = wrappedChild(); child )
        {
            auto const ratio = space.extent( 0 ) / space.extent( 1 );

            auto const subSpace = [ & ]() -> Range {
                if( ratio > m_ratio )
                {
                    return { space.min(),
                             { space.min( 0 ) + space.extent( 1 ) * m_ratio, space.max( 1 ) } };
                }
                else
                {
                    return { space.min(),
                             { space.max( 0 ), space.min( 1 ) + space.extent( 0 ) / m_ratio } };
                }
            }();

            return child->fit( subSpace );
        }

        return space;
    }
}
