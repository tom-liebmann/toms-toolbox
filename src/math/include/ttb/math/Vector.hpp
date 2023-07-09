#pragma once

#include <array>
#include <cassert>
#include <cstdlib>


namespace ttb
{
    template < typename TType, size_t TDim >
    class Vector
    {
    public:
        static constexpr size_t size = TDim;

        Vector() = default;

        Vector( Vector const& rhs ) = default;

        explicit Vector( TType const ( &values )[ TDim ] );

        template < typename TArg, typename = std::enable_if_t< TDim == 1, TArg > >
        explicit Vector( TArg&& values )
            : m_values{ static_cast< TType >( std::forward< TArg >( values ) ) }
        {
        }

        template < typename... TArgs,
                   typename = std::enable_if_t< TDim != 1 && TDim == sizeof...( TArgs ) > >
        Vector( TArgs&&... values )
            : m_values{ static_cast< TType >( std::forward< TArgs >( values ) )... }
        {
        }

        Vector& operator=( Vector const& rhs ) = default;

        // Access
        TType& operator[]( size_t index );

        TType const& operator[]( size_t index ) const;

        TType& operator()( size_t index );

        TType const& operator()( size_t index ) const;

        TType const* data() const;

        template < typename TType2 >
        Vector< TType2, TDim > as() const;

        // Special access
        TType& x();
        TType& y();
        TType& z();

        TType const& x() const;
        TType const& y() const;
        TType const& z() const;

    private:
        std::array< TType, TDim > m_values;
    };

    template < typename... TArgs >
    using CommonVectorElementType = std::decay_t< decltype( ( std::declval< TArgs >() + ... ) ) >;

    template < typename... TArgs >
    Vector( TArgs... ) -> Vector< CommonVectorElementType< TArgs... >, sizeof...( TArgs ) >;
}


// definitions
//=============================================================================

namespace ttb
{
    template < typename TType, size_t TDim >
    Vector< TType, TDim >::Vector( TType const ( &values )[ TDim ] )
    {
        std::copy( std::begin( values ), std::end( values ), std::begin( m_values ) );
    }

    template < typename TType, size_t TDim >
    inline TType& Vector< TType, TDim >::operator[]( size_t index )
    {
        assert( index < size );
        return m_values[ index ];
    }

    template < typename TType, size_t TDim >
    inline const TType& Vector< TType, TDim >::operator[]( size_t index ) const
    {
        assert( index < size );
        return m_values[ index ];
    }

    template < typename TType, size_t TDim >
    inline TType& Vector< TType, TDim >::operator()( size_t index )
    {
        assert( index < size );
        return m_values[ index ];
    }

    template < typename TType, size_t TDim >
    inline const TType& Vector< TType, TDim >::operator()( size_t index ) const
    {
        assert( index < size );
        return m_values[ index ];
    }

    template < typename TType, size_t TDim >
    TType const* Vector< TType, TDim >::data() const
    {
        return m_values.data();
    }

    template < typename TType, size_t TDim >
    template < typename TType2 >
    Vector< TType2, TDim > Vector< TType, TDim >::as() const
    {
        Vector< TType2, TDim > result;
        for( size_t i = 0; i < TDim; ++i )
        {
            result( i ) = static_cast< TType2 >( m_values[ i ] );
        }
        return result;
    }

    template < typename TType, size_t TDim >
    TType& Vector< TType, TDim >::x()
    {
        static_assert( size > 0, "Wrong vector type" );
        return m_values[ 0 ];
    }

    template < typename TType, size_t TDim >
    TType& Vector< TType, TDim >::y()
    {
        static_assert( size > 1, "Wrong vector type" );
        return m_values[ 1 ];
    }

    template < typename TType, size_t TDim >
    TType& Vector< TType, TDim >::z()
    {
        static_assert( size > 2, "Wrong vector type" );
        return m_values[ 2 ];
    }

    template < typename TType, size_t TDim >
    TType const& Vector< TType, TDim >::x() const
    {
        static_assert( size > 0, "Wrong vector type" );
        return m_values[ 0 ];
    }

    template < typename TType, size_t TDim >
    TType const& Vector< TType, TDim >::y() const
    {
        static_assert( size > 1, "Wrong vector type" );
        return m_values[ 1 ];
    }

    template < typename TType, size_t TDim >
    TType const& Vector< TType, TDim >::z() const
    {
        static_assert( size > 2, "Wrong vector type" );
        return m_values[ 2 ];
    }
}
