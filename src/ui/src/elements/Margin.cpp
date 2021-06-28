#include <ttb/ui/elements/Margin.hpp>

#include <ttb/math/matrix_operations.hpp>


namespace ttb::ui
{
    Margin::Margin( Framework& framework, float right, float top, float left, float bottom )
        : WrappedElement{ framework }
        , m_right{ right }
        , m_top{ top }
        , m_left{ left }
        , m_bottom{ bottom }
        , m_transform{ ttb::mat::identity< float, 3 >() }
    {
    }

    void Margin::child( Element* element )
    {
        wrappedChild(
            element,
            [ this ]( auto const& pos ) { return transform( pos ); },
            [ this ]( auto const& pos ) { return transformInv( pos ); } );
    }

    void Margin::right( float value, bool isLastChange )
    {
        m_right = value;

        if( isLastChange )
        {
            changed();
        }
    }

    void Margin::top( float value, bool isLastChange )
    {
        m_top = value;

        if( isLastChange )
        {
            changed();
        }
    }

    void Margin::left( float value, bool isLastChange )
    {
        m_left = value;

        if( isLastChange )
        {
            changed();
        }
    }

    void Margin::bottom( float value, bool isLastChange )
    {
        m_bottom = value;

        if( isLastChange )
        {
            changed();
        }
    }

    auto Margin::fit( Size const& size ) -> Size
    {
        if( auto const child = wrappedChild(); child )
        {
            auto const childSize =
                child->fit( { size( 0 ) - m_left - m_right, size( 1 ) - m_top - m_bottom } );

            m_transform( 0, 2 ) = m_left;
            m_transform( 1, 2 ) = m_top;

            return Element::fit(
                { childSize( 0 ) + m_left + m_right, childSize( 1 ) + m_top + m_bottom } );
        }

        return Element::fit( { 0.0f, 0.0f } );
    }

    void Margin::render( ttb::State& state ) const
    {
        if( auto const child = wrappedChild(); child )
        {
            auto const u =
                state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ).push( m_transform );

            child->render( state );
        }
    }

    auto Margin::transform( Position const& pos ) const -> Position
    {
        return { pos( 0 ) + m_left, pos( 1 ) + m_top };
    }

    auto Margin::transformInv( Position const& pos ) const -> Position
    {
        return { pos( 0 ) - m_left, pos( 1 ) - m_top };
    }
}
