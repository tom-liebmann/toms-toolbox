#include <ttb/ui/elements/core/Ratio.hpp>


namespace ttb::ui
{
    void Ratio::child( std::unique_ptr< Element > element )
    {
        wrappedChild( std::move( element ) );
    }

    auto Ratio::fit( Range const& space ) -> Range
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

        return wrappedChild()->fit( subSpace );
    }

    Ratio::Ratio( Framework& framework, float ratio )
        : ElementWrapper{ framework }, m_ratio{ ratio }
    {
    }
}
