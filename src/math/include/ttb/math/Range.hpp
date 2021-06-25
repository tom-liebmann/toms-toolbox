#pragma once

#include <ttb/math/Vector.hpp>

#include <iostream>



// declarations
//=============================================================================

namespace ttb
{
    template < typename T, size_t D >
    class Range
    {
    public:
        Range();

        Range( const Vector< T, D >& min, const Vector< T, D >& max );

        const T& getMin( size_t index ) const;
        const T& getMax( size_t index ) const;

        Vector< T, D > const& getMin() const;
        Vector< T, D > const& getMax() const;

        void min( size_t index, T const& value );
        void max( size_t index, T const& value );

        T const& min( size_t index ) const;
        T const& max( size_t index ) const;

        Vector< T, D > const& min() const;
        Vector< T, D > const& max() const;

        T extent( size_t index ) const;

        void extent( size_t index, T const& value );

        Vector< T, D > extent() const;

        T getBound( size_t index ) const;

        bool empty() const;

        bool contains( const Vector< T, D >& position ) const;

        /**
         * Extends the range such that it contains the given range.
         *
         * @rhs Range to be contained in this one
         */
        void extend( Range const& rhs );

    private:
        Vector< T, D > m_min;
        Vector< T, D > m_max;
    };



    template < typename T, size_t D >
    bool operator==( ttb::Range< T, D > const& lhs, ttb::Range< T, D > const& rhs );

    template < typename T, size_t D >
    bool operator!=( ttb::Range< T, D > const& lhs, ttb::Range< T, D > const& rhs );

    template < typename T, size_t D >
    Range< T, D > intersect( const Range< T, D >& lhs, const Range< T, D >& rhs );
}

template < typename T, size_t... D >
std::ostream& operator<<( std::ostream& stream, const ttb::Range< T, D... >& range );



// definitions
//=============================================================================

namespace ttb
{
    template < typename TType, size_t TDim >
    inline Range< TType, TDim >::Range()
    {
        for( size_t d = 0; d < TDim; ++d )
        {
            m_min( d ) = std::numeric_limits< TType >::max();
            m_max( d ) = std::numeric_limits< TType >::lowest();
        }
    }

    template < typename T, size_t D >
    inline Range< T, D >::Range( const Vector< T, D >& min, const Vector< T, D >& max )
        : m_min( min ), m_max( max )
    {
    }

    template < typename T, size_t D >
    inline const T& Range< T, D >::getMin( size_t index ) const
    {
        return m_min[ index ];
    }

    template < typename T, size_t D >
    inline const T& Range< T, D >::getMax( size_t index ) const
    {
        return m_max[ index ];
    }

    template < typename T, size_t D >
    inline Vector< T, D > const& Range< T, D >::getMin() const
    {
        return m_min;
    }

    template < typename T, size_t D >
    inline Vector< T, D > const& Range< T, D >::getMax() const
    {
        return m_max;
    }

    template < typename T, size_t D >
    void Range< T, D >::min( size_t index, T const& value )
    {
        m_min[ index ] = value;
    }

    template < typename T, size_t D >
    void Range< T, D >::max( size_t index, T const& value )
    {
        m_max[ index ] = value;
    }

    template < typename TType, size_t TDim >
    TType const& Range< TType, TDim >::min( size_t index ) const
    {
        return m_min[ index ];
    }

    template < typename TType, size_t TDim >
    TType const& Range< TType, TDim >::max( size_t index ) const
    {
        return m_max[ index ];
    }

    template < typename TType, size_t TDim >
    Vector< TType, TDim > const& Range< TType, TDim >::min() const
    {
        return m_min;
    }

    template < typename TType, size_t TDim >
    Vector< TType, TDim > const& Range< TType, TDim >::max() const
    {
        return m_max;
    }

    template < typename TType, size_t TDim >
    TType Range< TType, TDim >::extent( size_t index ) const
    {
        return m_max[ index ] - m_min[ index ];
    }

    template < typename TType, size_t TDim >
    void Range< TType, TDim >::extent( size_t index, TType const& value )
    {
        m_max[ index ] = m_min[ index ] + value;
    }

    template < typename TType, size_t TDim >
    Vector< TType, TDim > Range< TType, TDim >::extent() const
    {
        Vector< TType, TDim > result;

        for( size_t d = 0; d < TDim; ++d )
        {
            result( d ) = m_max( d ) - m_min( d );
        }

        return result;
    }

    template < typename T, size_t D >
    inline T Range< T, D >::getBound( size_t index ) const
    {
        return getMax( index ) - getMin( index );
    }

    template < typename T, size_t D >
    bool Range< T, D >::empty() const
    {
        for( size_t i = 0; i < D; ++i )
            if( getMin( i ) >= getMax( i ) )
                return true;
        return false;
    }

    template < typename T, size_t D >
    bool Range< T, D >::contains( const Vector< T, D >& position ) const
    {
        for( size_t i = 0; i < D; ++i )
            if( position[ i ] < m_min[ i ] || position[ i ] > m_max[ i ] )
                return false;
        return true;
    }

    template < typename TType, size_t TDim >
    void Range< TType, TDim >::extend( Range const& rhs )
    {
        for( size_t i = 0; i < TDim; ++i )
        {
            m_min( i ) = std::min( m_min( i ), rhs.min( i ) );
            m_max( i ) = std::max( m_max( i ), rhs.max( i ) );
        }
    }


    template < typename T, size_t D >
    bool operator==( ttb::Range< T, D > const& lhs, ttb::Range< T, D > const& rhs )
    {
        return lhs.getMin() == rhs.getMin() && lhs.getMax() == rhs.getMax();
    }

    template < typename T, size_t D >
    bool operator!=( ttb::Range< T, D > const& lhs, ttb::Range< T, D > const& rhs )
    {
        return !( lhs == rhs );
    }



    template < typename T, size_t D >
    inline Range< T, D > intersect( const Range< T, D >& lhs, const Range< T, D >& rhs )
    {
        using std::max;
        using std::min;
        return Range< T, D >( Vector< T, D >( { max( lhs.getMin( 0 ), rhs.getMin( 0 ) ),
                                                max( lhs.getMin( 1 ), rhs.getMin( 1 ) ) } ),
                              Vector< T, D >( { min( lhs.getMax( 0 ), rhs.getMax( 0 ) ),
                                                min( lhs.getMax( 1 ), rhs.getMax( 1 ) ) } ) );
    }
}

template < typename T, size_t... D >
std::ostream& operator<<( std::ostream& stream, const ttb::Range< T, D... >& range )
{
    return stream << range.getMin() << "-" << range.getMax();
}
