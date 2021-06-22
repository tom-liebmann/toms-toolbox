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

        Vector();

        template < typename... TArgs,
                   std::enable_if_t< TDim == 1 && TDim == sizeof...( TArgs ), int > = 0 >
        explicit Vector( TArgs... values ) : m_values{ values... }
        {
        }

        template < typename... TArgs,
                   std::enable_if_t< 1 < TDim && TDim == sizeof...( TArgs ), int > = 1 >
        Vector( TArgs... values ) : m_values{ values... }
        {
        }

        // Access
        TType& operator[]( size_t index );

        TType const& operator[]( size_t index ) const;

        TType& operator()( size_t index );

        TType const& operator()( size_t index ) const;

        TType const* data() const;

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
    Vector< TType, TDim >::Vector() = default;

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
