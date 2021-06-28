#include <ttb/ui/elements/ScaleToFit.hpp>

#include <ttb/math/Range.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>

#include <iostream>


namespace ttb::ui
{
    ScaleToFit::ScaleToFit( Framework& framework )
        : WrappedElement{ framework }, m_transform{ ttb::mat::identity< float, 3 >() }
    {
    }

    void ScaleToFit::child( Element* element )
    {
        wrappedChild(
            element,
            [ this ]( auto const& pos ) { return transform( pos ); },
            [ this ]( auto const& pos ) { return transformInv( pos ); } );
    }

    auto ScaleToFit::fit( Size const& size ) -> Size
    {
        if( auto const child = wrappedChild(); child )
        {
            auto const childSpace = child->fit( { 0.0f, 0.0f } );
            m_factor = std::min( size( 0 ) / childSpace( 0 ), size( 1 ) / childSpace( 1 ) );

            m_transform( 0, 0 ) = m_factor;
            m_transform( 1, 1 ) = m_factor;

            return Element::fit( { childSpace( 0 ) * m_factor, childSpace( 1 ) * m_factor } );
        }

        return Element::fit( { 0.0f, 0.0f } );
    }

    void ScaleToFit::render( ttb::State& state ) const
    {
        if( auto const child = wrappedChild(); child )
        {
            auto const u =
                state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ).push( m_transform );

            child->render( state );
        }
    }

    auto ScaleToFit::transform( Position const& pos ) const -> Position
    {
        return { pos( 0 ) * m_factor, pos( 1 ) * m_factor };
    }

    auto ScaleToFit::transformInv( Position const& pos ) const -> Position
    {
        return { pos( 0 ) / m_factor, pos( 1 ) / m_factor };
    }
}
