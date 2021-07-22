#include <ttb/ui/elements/Ratio.hpp>

#include <iostream>


namespace ttb::ui
{
    Ratio::Ratio( Framework& framework, float ratio )
        : WrappedElement{ framework }, m_ratio{ ratio }
    {
    }

    void Ratio::ratio( float ratio )
    {
        m_ratio = ratio;
        changed();
    }

    void Ratio::child( Element* element )
    {
        wrappedChild( element );
    }

    auto Ratio::fit( Size const& size ) -> Size
    {
        if( auto const child = wrappedChild(); child )
        {
            auto const ratio = size( 0 ) / size( 1 );

            auto const childSize =
                child->fit( ratio > m_ratio ? Size{ size( 1 ) * m_ratio, size( 1 ) }
                                            : Size{ size( 0 ), size( 0 ) / m_ratio } );

            return Element::fit( childSize );
        }

        return Element::fit( { 0.0f, 0.0f } );
    }
}
