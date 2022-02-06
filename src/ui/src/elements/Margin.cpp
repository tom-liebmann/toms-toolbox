#include <ttb/ui/elements/Margin.hpp>

#include <ttb/core/uniform.hpp>
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

    Margin::Margin( Framework& framework, float hMargin, float vMargin )
        : Margin{ framework, hMargin, vMargin, hMargin, vMargin }
    {
    }

    Margin::Margin( Framework& framework, float margin )
        : Margin{ framework, margin, margin, margin, margin }
    {
    }

    void Margin::child( Element* element )
    {
        wrappedChild(
            element,
            [ this ]( auto const& pos )
            {
                return transform( pos );
            },
            [ this ]( auto const& pos )
            {
                return transformInv( pos );
            } );
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
            state.with( ttb::UniformBinder{ "u_transform", m_transform },
                        [ & ]
                        {
                            child->render( state );
                        } );
        }
    }

    bool Margin::onEvent( ttb::Event const& event )
    {
        auto result = false;

        if( auto const child = wrappedChild(); child )
        {
            event.transform(
                [ this ]( auto const& v ) -> ttb::Vector< float, 2 >
                {
                    return { v( 0 ) - m_left, v( 1 ) - m_top };
                },
                [ &result, &child ]( auto const& event )
                {
                    result = child->onEvent( event );
                } );
        }

        return result;
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
