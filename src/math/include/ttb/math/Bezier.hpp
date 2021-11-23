#pragma once

#include "Vector.hpp"
#include "utils.hpp"
#include "vector_operations.hpp"

#include <cstdlib>
#include <vector>


namespace ttb
{
    template < typename TType, std::size_t TDim >
    class Bezier
    {
    public:
        Bezier( std::vector< ttb::Vector< TType, TDim > > points );

        ttb::Vector< TType, TDim > operator()( TType factor ) const;

        ttb::Vector< TType, TDim > derivative( TType factor ) const;

    private:
        std::vector< ttb::Vector< TType, TDim > > m_points;
    };
}


namespace ttb
{
    template < typename TType, std::size_t TDim >
    Bezier< TType, TDim >::Bezier( std::vector< ttb::Vector< TType, TDim > > points )
        : m_points( std::move( points ) )
    {
    }

    template < typename TType, std::size_t TDim >
    ttb::Vector< TType, TDim > Bezier< TType, TDim >::operator()( TType factor ) const
    {
        using std::pow;

        auto result = ttb::Vector< TType, TDim >{};

        for( std::size_t i = 0; i < TDim; ++i )
        {
            result( i ) = TType{ 0 };
        }

        auto const n = m_points.size() - 1;

        for( std::size_t i = 0; i <= n; ++i )
        {
            result += binomial( n, i ) * pow( TType{ 1 } - factor, n - i ) * pow( factor, i ) *
                      m_points[ i ];
        }

        return result;
    }

    template < typename TType, std::size_t TDim >
    ttb::Vector< TType, TDim > Bezier< TType, TDim >::derivative( TType factor ) const
    {
        using std::pow;

        auto result = ttb::Vector< TType, TDim >{};

        for( std::size_t i = 0; i < TDim; ++i )
        {
            result( i ) = TType{ 0 };
        }

        auto const n = m_points.size() - 1;

        for( std::size_t i = 0; i < n; ++i )
        {
            result += binomial( n - 1, i ) * pow( TType{ 1 } - factor, n - 1 - i ) *
                      pow( factor, i ) * ( m_points[ i + 1 ] - m_points[ i ] );
        }

        return n * result;
    }
}
