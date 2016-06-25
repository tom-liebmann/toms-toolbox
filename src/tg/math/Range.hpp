#pragma once

#include <tg/math/Vector.hpp>



// declarations
//=============================================================================

namespace tg
{
    template< typename T, size_t D >
    class Range
    {
    public:
        Range() = default;

        Range(
            const Vector< T, D >& min,
            const Vector< T, D >& max );

        const T& getMin( size_t index ) const;
        const T& getMax( size_t index ) const;

        Vector< T, D > const& getMinVector() const;
        Vector< T, D > const& getMaxVector() const;

        bool contains( const Vector< T, D >& position ) const;

    private:
        Vector< T, D > m_min;
        Vector< T, D > m_max;
    };



    template< typename T, size_t D >
    Range< T, D > intersect(
        const Range< T, D >& lhs,
        const Range< T, D >& rhs );
}



// definitions
//=============================================================================

namespace tg
{
    template< typename T, size_t D >
    inline Range< T, D >::Range(
        const Vector< T, D >& min,
        const Vector< T, D >& max )
        : m_min( min )
        , m_max( max )
    { }

    template< typename T, size_t D >
    inline const T& Range< T, D >::getMin( size_t index ) const
    {
        return m_min[ index ];
    }

    template< typename T, size_t D >
    inline const T& Range< T, D >::getMax( size_t index ) const
    {
        return m_max[ index ];
    }

    template< typename T, size_t D >
    inline Vector< T, D > const& Range< T, D >::getMinVector() const
    {
        return m_min;
    }

    template< typename T, size_t D >
    inline Vector< T, D > const& Range< T, D >::getMaxVector() const
    {
        return m_max;
    }

    template< typename T, size_t D >
    bool Range< T, D >::contains( const Vector< T, D >& position ) const
    {
        for( size_t i = 0; i < D; ++i )
            if( position[ i ] < m_min[ i ] || position[ i ] > m_max[ i ] )
                return false;
        return true;
    }

    template< typename T, size_t D >
    inline Range< T, D > intersect(
        const Range< T, D >& lhs,
        const Range< T, D >& rhs )
    {
        using namespace std;
        return Range< T, D >(
            Vector< T, D >( {
                max( lhs.getMin( 0 ), rhs.getMin( 0 ) ),
                max( lhs.getMin( 1 ), rhs.getMin( 1 ) )
            } ),
            Vector< T, D >( {
                min( lhs.getMax( 0 ), rhs.getMax( 0 ) ),
                min( lhs.getMax( 1 ), rhs.getMax( 1 ) )
            } )
        );
    }
}
