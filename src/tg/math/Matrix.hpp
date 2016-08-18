#pragma once

#include <tg/math/Tensor.hpp>

#include <tg/math/Vector.hpp>

#include <cstdlib>

// declarations
//=============================================================================

namespace tg
{
    template < typename T, size_t D1, size_t D2 >
    using Matrix = Tensor< T, D1, D2 >;

    template < typename T, size_t D1, size_t D2 >
    Matrix< T, D2, D1 > transpose( Matrix< T, D1, D2 > const& matrix );

    template < typename T, size_t D1, size_t D2, size_t D3 >
    Matrix< T, D1, D3 > operator*( Matrix< T, D1, D2 > const& lhs, Matrix< T, D2, D3 > const& rhs );

    template < typename T, size_t D1, size_t D2 >
    Vector< T, D1 > operator*( Matrix< T, D1, D2 > const& matrix, Vector< T, D2 > const& vector );
}



// definitions
//=================================================================================================

namespace tg
{
    template < typename T, size_t D1, size_t D2 >
    Matrix< T, D2, D1 > transpose( Matrix< T, D1, D2 > const& matrix )
    {
        Matrix< T, D2, D1 > result;

        for( size_t row = 0; row < D1; ++row )
            for( size_t col = 0; col < D2; ++col )
                result( col, row ) = matrix( row, col );

        return result;
    }

    template < typename T, size_t D1, size_t D2, size_t D3 >
    Matrix< T, D1, D3 > operator*( Matrix< T, D1, D2 > const& lhs, Matrix< T, D2, D3 > const& rhs )
    {
        Matrix< T, D1, D3 > result;

        for( size_t i = 0; i < D1; ++i )
        {
            for( size_t j = 0; j < D3; ++j )
            {
                result( i, j ) = 0;
                for( size_t k = 0; k < D2; ++k )
                    result( i, j ) += lhs( i, k ) * rhs( k, j );
            }
        }

        return result;
    }

    template < typename T, size_t D1, size_t D2 >
    Vector< T, D1 > operator*( Matrix< T, D1, D2 > const& matrix, Vector< T, D2 > const& vector )
    {
        Vector< T, D1 > result;

        for( size_t row = 0; row < D1; ++row )
        {
            result( row ) = 0;
            for( size_t col = 0; col < D2; ++col )
                result( row ) += matrix( row, col ) * vector( col );
        }

        return result;
    }
}
