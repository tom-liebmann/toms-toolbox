#pragma once

#include <math/TensorHelper.hpp>

#include <array>



// declarations
//=============================================================================

namespace tg
{
    template< typename T, size_t... D >
    class Tensor
    {
    public:
        static const size_t size = tg_private::pack_product< D... >::value;
        static const size_t order = sizeof...( D );

        Tensor();

        Tensor( std::initializer_list< T > values );

        // Access
        T& operator[]( size_t index );

        const T& operator[]( size_t index ) const;

        template< typename... I >
        T& operator()( I... indices );

        template< typename... I >
        const T& operator()( I... indices ) const;

        // Special access
        typename std::enable_if< order == 1 && 0 < size, T& >::type x();
        typename std::enable_if< order == 1 && 1 < size, T& >::type y();
        typename std::enable_if< order == 1 && 2 < size, T& >::type z();

        typename std::enable_if< order == 1 && 0 < size, const T& >::type x() const;
        typename std::enable_if< order == 1 && 1 < size, const T& >::type y() const;
        typename std::enable_if< order == 1 && 2 < size, const T& >::type z() const;

    private:
        std::array< T, size > m_values;
    };
}



template< typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator+(
    const tg::Tensor< T1, D... >& lhs, const tg::Tensor< T2, D... >& rhs );

template< typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator-(
    const tg::Tensor< T1, D... >& lhs, const tg::Tensor< T2, D... >& rhs );

template< typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator+=(
    tg::Tensor< T1, D... >& lhs, const tg::Tensor< T2, D... >& rhs );

template< typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator-=(
    tg::Tensor< T1, D... >& lhs, const tg::Tensor< T2, D... >& rhs );

template< typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator*(
    const tg::Tensor< T1, D... >& lhs, T2 rhs );

template< typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator*(
    T1 lhs, const tg::Tensor< T2, D... >& rhs );

template< typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator/(
    const tg::Tensor< T1, D... >& lhs, const T2& rhs );

template< typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator*=(
    tg::Tensor< T1, D... >& lhs, const T2& rhs );

template< typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator/=(
    tg::Tensor< T1, D... >& lhs, const T2& rhs );



// definitions
//=============================================================================

namespace tg
{
    template< typename T, size_t... D >
    inline Tensor< T, D... >::Tensor()
    { }

    template< typename T, size_t... D >
    inline Tensor< T, D... >::Tensor( std::initializer_list< T > values )
    {
        std::copy( values.begin(), values.end(), m_values.begin() );
    }

    template< typename T, size_t... D >
    inline T& Tensor< T, D... >::operator[]( size_t index )
    {
        return m_values[ index ];
    }

    template< typename T, size_t... D >
    inline const T& Tensor< T, D... >::operator[]( size_t index ) const
    {
        return m_values[ index ];
    }

    template< typename T, size_t... D >
    template< typename... I >
    inline T& Tensor< T, D... >::operator()( I... indices )
    {
        return m_values[ tg_private::arg_product< D... >::value( indices... ) ];
    }

    template< typename T, size_t... D >
    template< typename... I >
    inline const T& Tensor< T, D... >::operator()( I... indices ) const
    {
        return m_values[ tg_private::arg_product< D... >::value( indices... ) ];
    }

    template< typename T, size_t... D >
    typename std::enable_if< Tensor< T, D... >::order == 1 && 0 < Tensor< T, D... >::size, T& >::type Tensor< T, D... >::x()
    {
        return m_values[ 0 ];
    }

    template< typename T, size_t... D >
    typename std::enable_if< Tensor< T, D... >::order == 1 && 1 < Tensor< T, D... >::size, T& >::type Tensor< T, D... >::y()
    {
        return m_values[ 1 ];
    }

    template< typename T, size_t... D >
    typename std::enable_if< Tensor< T, D... >::order == 1 && 2 < Tensor< T, D... >::size, T& >::type Tensor< T, D... >::z()
    {
        return m_values[ 2 ];
    }

    template< typename T, size_t... D >
    typename std::enable_if< Tensor< T, D... >::order == 1 && 0 < Tensor< T, D... >::size, const T& >::type Tensor< T, D... >::x() const
    {
        return m_values[ 0 ];
    }

    template< typename T, size_t... D >
    typename std::enable_if< Tensor< T, D... >::order == 1 && 1 < Tensor< T, D... >::size, const T& >::type Tensor< T, D... >::y() const
    {
        return m_values[ 1 ];
    }

    template< typename T, size_t... D >
    typename std::enable_if< Tensor< T, D... >::order == 1 && 2 < Tensor< T, D... >::size, const T& >::type Tensor< T, D... >::z() const
    {
        return m_values[ 2 ];
    }
}



template< typename T1, typename T2, size_t... D >
inline tg::Tensor< decltype( T1() * T2() ), D... > operator+(
    const tg::Tensor< T1, D... >& lhs, const tg::Tensor< T2, D... >& rhs )
{
    tg::Tensor< decltype( T1() * T2() ), D... > result;

    for( size_t i = 0; i < result.size; ++i )
        result[ i ] = lhs[ i ] + rhs[ i ];

    return result;
}

template< typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator-(
    const tg::Tensor< T1, D... >& lhs, const tg::Tensor< T2, D... >& rhs )
{
    tg::Tensor< decltype( T1() * T2() ), D... > result;

    for( size_t i = 0; i < result.size; ++i )
        result[ i ] = lhs[ i ] - rhs[ i ];

    return result;
}

template< typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator+=(
    tg::Tensor< T1, D... >& lhs, const tg::Tensor< T2, D... >& rhs )
{
    for( size_t i = 0; i < lhs.size; ++i )
        lhs[ i ] += rhs[ i ];

    return lhs;
}

template< typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator-=(
    tg::Tensor< T1, D... >& lhs, const tg::Tensor< T2, D... >& rhs )
{
    for( size_t i = 0; i < lhs.size; ++i )
        lhs[ i ] -= rhs[ i ];

    return lhs;
}

template< typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator*(
    const tg::Tensor< T1, D... >& lhs, T2 rhs )
{
    tg::Tensor< decltype( T1() * T2() ), D... > result;

    for( size_t i = 0; i < lhs.size; ++i )
        result[ i ] = lhs[ i ] * rhs;

    return result;
}

template< typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator*(
    T1 lhs, const tg::Tensor< T2, D... >& rhs )
{
    tg::Tensor< decltype( T1() * T2() ), D... > result;

    for( size_t i = 0; i < rhs.size; ++i )
        result[ i ] = lhs * rhs[ i ];

    return result;
}

template< typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator/(
    const tg::Tensor< T1, D... >& lhs, const T2& rhs )
{
    tg::Tensor< decltype( T1() * T2() ), D... > result;

    for( size_t i = 0; i < lhs.size; ++i )
        result[ i ] = lhs[ i ] / rhs;

    return result;
}

template< typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator*=(
    tg::Tensor< T1, D... >& lhs, const T2& rhs )
{
    for( size_t i = 0; i < lhs.size; ++i )
        lhs[ i ] *= rhs;

    return lhs;
}

template< typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator/=(
    tg::Tensor< T1, D... >& lhs, const T2& rhs )
{
    for( size_t i = 0; i < lhs.size; ++i )
        lhs[ i ] /= rhs;

    return lhs;
}
