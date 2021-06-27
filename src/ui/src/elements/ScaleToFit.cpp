#include <ttb/ui/elements/ScaleToFit.hpp>

#include <ttb/math/Range.hpp>
#include <ttb/math/vector_operations.hpp>

#include <iostream>


namespace ttb::ui
{
    ScaleToFit::ScaleToFit( Framework& framework ) : WrappedElement{ framework }
    {
    }

    void ScaleToFit::child( Element* element )
    {
        wrappedChild( element );
    }

    auto ScaleToFit::fit( Range const& space ) -> Range
    {
        if( auto const child = wrappedChild(); child )
        {
            auto const childSpace = child->fit( space );

            m_factor = std::min( space.extent( 0 ) / childSpace.extent( 0 ),
                                 space.extent( 1 ) / childSpace.extent( 1 ) );

            auto const result =
                Range{ space.min(),
                       space.min() + ttb::Vector{ childSpace.extent( 0 ) * m_factor,
                                                  childSpace.extent( 1 ) * m_factor } };

            return result;
        }

        return space;
    }

    void ScaleToFit::range( Range const& value )
    {
        Element::range( value );

        auto const childRange = Range{
            { 0.0f, 0.0f },
        };
    }

    void ScaleToFit::render( ttb::State& state ) const
    {
    }
}
