#include <ttb/ui/elements/ScaleToFit.hpp>

#include <ttb/core/uniform.hpp>
#include <ttb/math/Range.hpp>
#include <ttb/math/matrix_operations.hpp>
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

    auto ScaleToFit::fit( Size const& size ) -> Size
    {
        if( auto const child = wrappedChild(); child )
        {
            auto const childSpace = child->fit( { 0.0f, 0.0f } );
            m_factor = std::min( size( 0 ) / childSpace( 0 ), size( 1 ) / childSpace( 1 ) );

            return Element::fit( { childSpace( 0 ) * m_factor, childSpace( 1 ) * m_factor } );
        }

        return Element::fit( { 0.0f, 0.0f } );
    }

    void ScaleToFit::render( ttb::State& state ) const
    {
        if( auto const child = wrappedChild(); child )
        {
            child->render( state );
        }
    }
}
