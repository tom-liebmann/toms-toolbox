#pragma once

#include <tg/math/TensorHelper.hpp>

#include <array>
#include <iostream>



// declarations
//=============================================================================

namespace tg
{
    template < typename T, size_t... D >
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

        template < typename... I >
        T& operator()( I... indices );

        template < typename... I >
        const T& operator()( I... indices ) const;

        // Special access
        T& x();
        T& y();
        T& z();

        const T& x() const;
        const T& y() const;
        const T& z() const;

    private:
        std::array< T, size > m_values;
    };

    template < typename T, size_t... D >
    bool operator==( tg::Tensor< T, D... > const& lhs, tg::Tensor< T, D... > const& rhs );

    template < typename T, size_t... D >
    bool operator!=( tg::Tensor< T, D... > const& lhs, tg::Tensor< T, D... > const& rhs );
}



template < typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator+( const tg::Tensor< T1, D... >& lhs,
                                                       const tg::Tensor< T2, D... >& rhs );

template < typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator-( const tg::Tensor< T1, D... >& lhs,
                                                       const tg::Tensor< T2, D... >& rhs );

template < typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator+=( tg::Tensor< T1, D... >& lhs,
                                    const tg::Tensor< T2, D... >& rhs );

template < typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator-=( tg::Tensor< T1, D... >& lhs,
                                    const tg::Tensor< T2, D... >& rhs );

template < typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator*( const tg::Tensor< T1, D... >& lhs, T2 rhs );

template < typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator*( T1 lhs, const tg::Tensor< T2, D... >& rhs );

template < typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator/( const tg::Tensor< T1, D... >& lhs,
                                                       const T2& rhs );

template < typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator*=( tg::Tensor< T1, D... >& lhs, const T2& rhs );

template < typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator/=( tg::Tensor< T1, D... >& lhs, const T2& rhs );

template < typename T, size_t... D >
std::ostream& operator<<( std::ostream& stream, const tg::Tensor< T, D... >& tensor );



// definitions
//=============================================================================

namespace tg
{
    template < typename T, size_t... D >
    inline Tensor< T, D... >::Tensor()
    {
    }

    template < typename T, size_t... D >
    inline Tensor< T, D... >::Tensor( std::initializer_list< T > values )
    {
        std::copy( values.begin(), values.end(), m_values.begin() );
    }

    template < typename T, size_t... D >
    inline T& Tensor< T, D... >::operator[]( size_t index )
    {
        return m_values[ index ];
    }

    template < typename T, size_t... D >
    inline const T& Tensor< T, D... >::operator[]( size_t index ) const
    {
        return m_values[ index ];
    }

    template < typename T, size_t... D >
    template < typename... I >
    inline T& Tensor< T, D... >::operator()( I... indices )
    {
        return m_values[ tg_private::arg_product< D... >::value( indices... ) ];
    }

    template < typename T, size_t... D >
    template < typename... I >
    inline const T& Tensor< T, D... >::operator()( I... indices ) const
    {
        return m_values[ tg_private::arg_product< D... >::value( indices... ) ];
    }

    template < typename T, size_t... D >
    T& Tensor< T, D... >::x()
    {
        static_assert( size > 0 && order == 1, "Wrong tensor type!" );
        return m_values[ 0 ];
    }

    template < typename T, size_t... D >
    T& Tensor< T, D... >::y()
    {
        static_assert( size > 1 && order == 1, "Wrong tensor type!" );
        return m_values[ 1 ];
    }

    template < typename T, size_t... D >
    T& Tensor< T, D... >::z()
    {
        static_assert( size > 2 && order == 1, "Wrong tensor type!" );
        return m_values[ 2 ];
    }

    template < typename T, size_t... D >
    const T& Tensor< T, D... >::x() const
    {
        static_assert( size > 0 && order == 1, "Wrong tensor type!" );
        return m_values[ 0 ];
    }

    template < typename T, size_t... D >
    const T& Tensor< T, D... >::y() const
    {
        static_assert( size > 1 && order == 1, "Wrong tensor type!" );
        return m_values[ 1 ];
    }

    template < typename T, size_t... D >
    const T& Tensor< T, D... >::z() const
    {
        static_assert( size > 2 && order == 1, "Wrong tensor type!" );
        return m_values[ 2 ];
    }

    template < typename T, size_t... D >
    bool operator==( tg::Tensor< T, D... > const& lhs, tg::Tensor< T, D... > const& rhs )
    {
        for( size_t i = 0; i < lhs.size; ++i )
            if( lhs[ i ] != rhs[ i ] )
                return false;
        return true;
    }

    template < typename T, size_t... D >
    bool operator!=( tg::Tensor< T, D... > const& lhs, tg::Tensor< T, D... > const& rhs )
    {
        for( size_t i = 0; i < lhs.size; ++i )
            if( lhs[ i ] != rhs[ i ] )
                return true;
        return false;
    }
}



template < typename T1, typename T2, size_t... D >
inline tg::Tensor< decltype( T1() * T2() ), D... > operator+( const tg::Tensor< T1, D... >& lhs,
                                                              const tg::Tensor< T2, D... >& rhs )
{
    tg::Tensor< decltype( T1() * T2() ), D... > result;

    for( size_t i = 0; i < result.size; ++i )
        result[ i ] = lhs[ i ] + rhs[ i ];

    return result;
}

template < typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator-( const tg::Tensor< T1, D... >& lhs,
                                                       const tg::Tensor< T2, D... >& rhs )
{
    tg::Tensor< decltype( T1() * T2() ), D... > result;

    for( size_t i = 0; i < result.size; ++i )
        result[ i ] = lhs[ i ] - rhs[ i ];

    return result;
}

template < typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator+=( tg::Tensor< T1, D... >& lhs, const tg::Tensor< T2, D... >& rhs )
{
    for( size_t i = 0; i < lhs.size; ++i )
        lhs[ i ] += rhs[ i ];

    return lhs;
}

template < typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator-=( tg::Tensor< T1, D... >& lhs, const tg::Tensor< T2, D... >& rhs )
{
    for( size_t i = 0; i < lhs.size; ++i )
        lhs[ i ] -= rhs[ i ];

    return lhs;
}

template < typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator*( const tg::Tensor< T1, D... >& lhs, T2 rhs )
{
    tg::Tensor< decltype( T1() * T2() ), D... > result;

    for( size_t i = 0; i < lhs.size; ++i )
        result[ i ] = lhs[ i ] * rhs;

    return result;
}

template < typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator*( T1 lhs, const tg::Tensor< T2, D... >& rhs )
{
    tg::Tensor< decltype( T1() * T2() ), D... > result;

    for( size_t i = 0; i < rhs.size; ++i )
        result[ i ] = lhs * rhs[ i ];

    return result;
}

template < typename T1, typename T2, size_t... D >
tg::Tensor< decltype( T1() * T2() ), D... > operator/( const tg::Tensor< T1, D... >& lhs,
                                                       const T2& rhs )
{
    tg::Tensor< decltype( T1() * T2() ), D... > result;

    for( size_t i = 0; i < lhs.size; ++i )
        result[ i ] = lhs[ i ] / rhs;

    return result;
}

template < typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator*=( tg::Tensor< T1, D... >& lhs, const T2& rhs )
{
    for( size_t i = 0; i < lhs.size; ++i )
        lhs[ i ] *= rhs;

    return lhs;
}

template < typename T1, typename T2, size_t... D >
tg::Tensor< T1, D... >& operator/=( tg::Tensor< T1, D... >& lhs, const T2& rhs )
{
    for( size_t i = 0; i < lhs.size; ++i )
        lhs[ i ] /= rhs;

    return lhs;
}

template < class T, size_t... D >
std::ostream& operator<<( std::ostream& stream, const tg::Tensor< T, D... >& tensor )
{
    stream << "(";
    for( size_t i = 0; i < tensor.size; ++i )
    {
        if( i != 0 )
            stream << ", ";
        stream << tensor[ i ];
    }
    stream << ")";
}
