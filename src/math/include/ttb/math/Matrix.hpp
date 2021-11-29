#pragma once

#include <array>
#include <cassert>
#include <cstdlib>
#include <type_traits>


namespace ttb
{
    template < typename TType, size_t TRows, size_t TCols >
    class Matrix
    {
    public:
        static constexpr size_t size = TRows * TCols;
        static constexpr size_t rows = TRows;
        static constexpr size_t cols = TCols;

        Matrix();

        template < typename... TArgs,
                   std::enable_if_t< 1 == size && size == sizeof...( TArgs ), int > = 0 >
        explicit Matrix( TArgs... values ) : m_values{ values... }
        {
        }

        template < typename... TArgs,
                   std::enable_if_t< 1 < size && size == sizeof...( TArgs ), int > = 1 >
        Matrix( TArgs... values ) : m_values{ values... }
        {
        }

        // Access
        TType& operator[]( size_t index );

        TType const& operator[]( size_t index ) const;

        TType& operator()( size_t row, size_t col );

        TType const& operator()( size_t row, size_t col ) const;

        TType const* data() const;

    private:
        std::array< TType, TRows * TCols > m_values;
    };
}


// definitions
//=============================================================================

namespace ttb
{
    template < typename TType, size_t TRows, size_t TCols >
    Matrix< TType, TRows, TCols >::Matrix() : m_values{}
    {
    }

    template < typename TType, size_t TRows, size_t TCols >
    inline TType& Matrix< TType, TRows, TCols >::operator[]( size_t index )
    {
        assert( index < size );
        return m_values[ index ];
    }

    template < typename TType, size_t TRows, size_t TCols >
    inline const TType& Matrix< TType, TRows, TCols >::operator[]( size_t index ) const
    {
        assert( index < size );
        return m_values[ index ];
    }

    template < typename TType, size_t TRows, size_t TCols >
    inline TType& Matrix< TType, TRows, TCols >::operator()( size_t row, size_t col )
    {
        assert( row < rows );
        assert( col < cols );
        return m_values[ col + row * cols ];
    }

    template < typename TType, size_t TRows, size_t TCols >
    inline const TType& Matrix< TType, TRows, TCols >::operator()( size_t row, size_t col ) const
    {
        assert( row < rows );
        assert( col < cols );
        return m_values[ col + row * cols ];
    }

    template < typename TType, size_t TRows, size_t TCols >
    TType const* Matrix< TType, TRows, TCols >::data() const
    {
        return m_values.data();
    }
}
