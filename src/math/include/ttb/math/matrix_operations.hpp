#pragma once

#include "Matrix.hpp"
#include "Quaternion.hpp"
#include "Range.hpp"
#include "Vector.hpp"

#include <cstdlib>
#include <iostream>

// declarations
//=============================================================================

namespace ttb
{
    using Matrix4f = Matrix< float, 4, 4 >;


    template < typename TType, size_t TRows, size_t TCols >
    Matrix< TType, TCols, TRows > transpose( Matrix< TType, TRows, TCols > const& matrix );

    template < typename TType >
    Matrix< TType, 1, 1 > invert( Matrix< TType, 1, 1 > const& matrix );

    template < typename TType >
    Matrix< TType, 2, 2 > invert( Matrix< TType, 2, 2 > const& matrix );

    template < typename TType >
    Matrix< TType, 3, 3 > invert( Matrix< TType, 3, 3 > const& matrix );

    template < typename TType >
    Matrix< TType, 4, 4 > invert( Matrix< TType, 4, 4 > const& matrix );

    template < typename TType1, typename TType2, size_t TRows1, size_t TCols1, size_t TCols2 >
    auto operator*( Matrix< TType1, TRows1, TCols1 > const& lhs,
                    Matrix< TType2, TCols1, TCols2 > const& rhs );

    template < typename TType1,
               typename TType2,
               size_t TRows,
               size_t TCols,
               typename Enabled = typename std::enable_if_t< std::is_arithmetic_v< TType1 > > >
    auto operator*( TType1 lhs, Matrix< TType2, TRows, TCols > const& rhs );

    template < typename TType1, typename TType2, size_t TRows, size_t TCols >
    auto operator+( Matrix< TType1, TRows, TCols > const& lhs,
                    Matrix< TType2, TRows, TCols > const& rhs );

    template < typename TType1, typename TType2, size_t TRows, size_t TCols >
    auto operator-( Matrix< TType1, TRows, TCols > const& lhs,
                    Matrix< TType2, TRows, TCols > const& rhs );

    template < typename TType1, typename TType2, size_t TRows, size_t TCols >
    Matrix< TType1, TRows, TCols >& operator*=( Matrix< TType1, TRows, TCols >& lhs,
                                                Matrix< TType2, TCols, TCols > const& rhs );

    template < typename TType1, typename TType2, size_t TRows, size_t TCols >
    auto operator*( Matrix< TType1, TRows, TCols > const& matrix,
                    Vector< TType2, TCols > const& vector );

    template < typename TType1, typename TType2, size_t TDim >
    auto operator*( Matrix< TType1, TDim + 1, TDim + 1 > const& matrix,
                    Vector< TType2, TDim > const& vector );

    template < typename TType, size_t TRows, size_t TCols >
    std::ostream& operator<<( std::ostream& stream, Matrix< TType, TRows, TCols > const& mat );

    namespace mat
    {
        template < typename TType, size_t TDim >
        Matrix< TType, TDim, TDim > identity();

        template < typename TType, size_t TDim >
        Matrix< TType, TDim + 1, TDim + 1 > translation( Vector< TType, TDim > const& vec );

        template < typename TType >
        Matrix< TType, 3, 3 > rotation( TType angle );

        template < typename TType >
        Matrix< TType, 4, 4 > rotation( Quaternion< TType > const& quat );

        template < typename TType, size_t TDim >
        Matrix< TType, TDim + 1, TDim + 1 > scale( Vector< TType, TDim > const& vec );

        template < typename TType, size_t TDim >
        Matrix< TType, TDim + 1, TDim + 1 > transform( Range< TType, TDim > const& from,
                                                       Range< TType, TDim > const& to );

        template < typename TType >
        Matrix< TType, 4, 4 >
            ortho( TType right, TType top, TType left, TType bottom, TType zNear, TType zFar );

        template < typename TType >
        Matrix< TType, 4, 4 > perspective( TType fovx, TType aspect, TType zNear, TType zFar );

        template < typename TType >
        Matrix< TType, 4, 4 > perspectiveInv( TType fovx, TType aspect, TType zNear, TType zFar );

        template < typename TType >
        Matrix< TType, 4, 4 > lookAt( Vector< TType, 3 > const& from,
                                      Vector< TType, 3 > const& to,
                                      Vector< TType, 3 > const& up );

        template < typename TType >
        Matrix< TType, 4, 4 > lookAtInv( Vector< TType, 3 > const& from,
                                         Vector< TType, 3 > const& to,
                                         Vector< TType, 3 > const& up );
    }

    template < typename TType >
    struct is_matrix : public std::false_type
    {
    };

    template < typename TType, std::size_t TRows, std::size_t TCols >
    struct is_matrix< Matrix< TType, TRows, TCols > > : public std::true_type
    {
    };

    template < typename TType >
    constexpr auto is_matrix_v = is_matrix< TType >::value;
}



// definitions
//=================================================================================================

namespace ttb
{
    template < typename TType, size_t TRows, size_t TCols >
    Matrix< TType, TCols, TRows > transpose( Matrix< TType, TRows, TCols > const& matrix )
    {
        Matrix< TType, TCols, TRows > result;

        for( size_t row = 0; row < TRows; ++row )
        {
            for( size_t col = 0; col < TCols; ++col )
            {
                result( col, row ) = matrix( row, col );
            }
        }

        return result;
    }

    template < typename TType >
    Matrix< TType, 1, 1 > invert( Matrix< TType, 1, 1 > const& m )
    {
        return Matrix< TType, 1, 1 >{ TType{ 1 } / m( 0, 0 ) };
    }

    template < typename TType >
    Matrix< TType, 2, 2 > invert( Matrix< TType, 2, 2 > const& m )
    {
        auto const det = m( 0, 0 ) * m( 1, 1 ) - m( 1, 0 ) * m( 0, 1 );
        return Matrix< TType, 2, 2 >{
            m( 1, 1 ) / det, -m( 0, 1 ) / det, -m( 1, 0 ) / det, m( 0, 0 ) / det
        };
    }

    template < typename TType >
    Matrix< TType, 3, 3 > invert( Matrix< TType, 3, 3 > const& m )
    {
        Matrix< TType, 3, 3 > r;

        auto const det = m( 0, 0 ) * ( m( 1, 1 ) * m( 2, 2 ) - m( 2, 1 ) * m( 1, 2 ) ) -
                         m( 0, 1 ) * ( m( 1, 0 ) * m( 2, 2 ) - m( 1, 2 ) * m( 2, 0 ) ) +
                         m( 0, 2 ) * ( m( 1, 0 ) * m( 2, 1 ) - m( 1, 1 ) * m( 2, 0 ) );

        r( 0, 0 ) = ( m( 1, 1 ) * m( 2, 2 ) - m( 2, 1 ) * m( 1, 2 ) ) * det;
        r( 0, 1 ) = ( m( 0, 2 ) * m( 2, 1 ) - m( 0, 1 ) * m( 2, 2 ) ) * det;
        r( 0, 2 ) = ( m( 0, 1 ) * m( 1, 2 ) - m( 0, 2 ) * m( 1, 1 ) ) * det;
        r( 1, 0 ) = ( m( 1, 2 ) * m( 2, 0 ) - m( 1, 0 ) * m( 2, 2 ) ) * det;
        r( 1, 1 ) = ( m( 0, 0 ) * m( 2, 2 ) - m( 0, 2 ) * m( 2, 0 ) ) * det;
        r( 1, 2 ) = ( m( 1, 0 ) * m( 0, 2 ) - m( 0, 0 ) * m( 1, 2 ) ) * det;
        r( 2, 0 ) = ( m( 1, 0 ) * m( 2, 1 ) - m( 2, 0 ) * m( 1, 1 ) ) * det;
        r( 2, 1 ) = ( m( 2, 0 ) * m( 0, 1 ) - m( 0, 0 ) * m( 2, 1 ) ) * det;
        r( 2, 2 ) = ( m( 0, 0 ) * m( 1, 1 ) - m( 1, 0 ) * m( 0, 1 ) ) * det;

        return r;
    }

    template < typename TType >
    Matrix< TType, 4, 4 > invert( Matrix< TType, 4, 4 > const& matrix )
    {
        using std::abs;

        auto const& m = matrix;

        Matrix< TType, 4, 4 > result;

        result( 0, 0 ) = m[ 5 ] * m[ 10 ] * m[ 15 ] - m[ 5 ] * m[ 11 ] * m[ 14 ] -
                         m[ 9 ] * m[ 6 ] * m[ 15 ] + m[ 9 ] * m[ 7 ] * m[ 14 ] +
                         m[ 13 ] * m[ 6 ] * m[ 11 ] - m[ 13 ] * m[ 7 ] * m[ 10 ];

        result( 1, 0 ) = -m[ 4 ] * m[ 10 ] * m[ 15 ] + m[ 4 ] * m[ 11 ] * m[ 14 ] +
                         m[ 8 ] * m[ 6 ] * m[ 15 ] - m[ 8 ] * m[ 7 ] * m[ 14 ] -
                         m[ 12 ] * m[ 6 ] * m[ 11 ] + m[ 12 ] * m[ 7 ] * m[ 10 ];

        result( 2, 0 ) = m[ 4 ] * m[ 9 ] * m[ 15 ] - m[ 4 ] * m[ 11 ] * m[ 13 ] -
                         m[ 8 ] * m[ 5 ] * m[ 15 ] + m[ 8 ] * m[ 7 ] * m[ 13 ] +
                         m[ 12 ] * m[ 5 ] * m[ 11 ] - m[ 12 ] * m[ 7 ] * m[ 9 ];

        result( 3, 0 ) = -m[ 4 ] * m[ 9 ] * m[ 14 ] + m[ 4 ] * m[ 10 ] * m[ 13 ] +
                         m[ 8 ] * m[ 5 ] * m[ 14 ] - m[ 8 ] * m[ 6 ] * m[ 13 ] -
                         m[ 12 ] * m[ 5 ] * m[ 10 ] + m[ 12 ] * m[ 6 ] * m[ 9 ];

        result( 0, 1 ) = -m[ 1 ] * m[ 10 ] * m[ 15 ] + m[ 1 ] * m[ 11 ] * m[ 14 ] +
                         m[ 9 ] * m[ 2 ] * m[ 15 ] - m[ 9 ] * m[ 3 ] * m[ 14 ] -
                         m[ 13 ] * m[ 2 ] * m[ 11 ] + m[ 13 ] * m[ 3 ] * m[ 10 ];

        result( 1, 1 ) = m[ 0 ] * m[ 10 ] * m[ 15 ] - m[ 0 ] * m[ 11 ] * m[ 14 ] -
                         m[ 8 ] * m[ 2 ] * m[ 15 ] + m[ 8 ] * m[ 3 ] * m[ 14 ] +
                         m[ 12 ] * m[ 2 ] * m[ 11 ] - m[ 12 ] * m[ 3 ] * m[ 10 ];

        result( 2, 1 ) = -m[ 0 ] * m[ 9 ] * m[ 15 ] + m[ 0 ] * m[ 11 ] * m[ 13 ] +
                         m[ 8 ] * m[ 1 ] * m[ 15 ] - m[ 8 ] * m[ 3 ] * m[ 13 ] -
                         m[ 12 ] * m[ 1 ] * m[ 11 ] + m[ 12 ] * m[ 3 ] * m[ 9 ];

        result( 3, 1 ) = m[ 0 ] * m[ 9 ] * m[ 14 ] - m[ 0 ] * m[ 10 ] * m[ 13 ] -
                         m[ 8 ] * m[ 1 ] * m[ 14 ] + m[ 8 ] * m[ 2 ] * m[ 13 ] +
                         m[ 12 ] * m[ 1 ] * m[ 10 ] - m[ 12 ] * m[ 2 ] * m[ 9 ];

        result( 0, 2 ) = m[ 1 ] * m[ 6 ] * m[ 15 ] - m[ 1 ] * m[ 7 ] * m[ 14 ] -
                         m[ 5 ] * m[ 2 ] * m[ 15 ] + m[ 5 ] * m[ 3 ] * m[ 14 ] +
                         m[ 13 ] * m[ 2 ] * m[ 7 ] - m[ 13 ] * m[ 3 ] * m[ 6 ];

        result( 1, 2 ) = -m[ 0 ] * m[ 6 ] * m[ 15 ] + m[ 0 ] * m[ 7 ] * m[ 14 ] +
                         m[ 4 ] * m[ 2 ] * m[ 15 ] - m[ 4 ] * m[ 3 ] * m[ 14 ] -
                         m[ 12 ] * m[ 2 ] * m[ 7 ] + m[ 12 ] * m[ 3 ] * m[ 6 ];

        result( 2, 2 ) = m[ 0 ] * m[ 5 ] * m[ 15 ] - m[ 0 ] * m[ 7 ] * m[ 13 ] -
                         m[ 4 ] * m[ 1 ] * m[ 15 ] + m[ 4 ] * m[ 3 ] * m[ 13 ] +
                         m[ 12 ] * m[ 1 ] * m[ 7 ] - m[ 12 ] * m[ 3 ] * m[ 5 ];

        result( 3, 2 ) = -m[ 0 ] * m[ 5 ] * m[ 14 ] + m[ 0 ] * m[ 6 ] * m[ 13 ] +
                         m[ 4 ] * m[ 1 ] * m[ 14 ] - m[ 4 ] * m[ 2 ] * m[ 13 ] -
                         m[ 12 ] * m[ 1 ] * m[ 6 ] + m[ 12 ] * m[ 2 ] * m[ 5 ];

        result( 0, 3 ) = -m[ 1 ] * m[ 6 ] * m[ 11 ] + m[ 1 ] * m[ 7 ] * m[ 10 ] +
                         m[ 5 ] * m[ 2 ] * m[ 11 ] - m[ 5 ] * m[ 3 ] * m[ 10 ] -
                         m[ 9 ] * m[ 2 ] * m[ 7 ] + m[ 9 ] * m[ 3 ] * m[ 6 ];

        result( 1, 3 ) = m[ 0 ] * m[ 6 ] * m[ 11 ] - m[ 0 ] * m[ 7 ] * m[ 10 ] -
                         m[ 4 ] * m[ 2 ] * m[ 11 ] + m[ 4 ] * m[ 3 ] * m[ 10 ] +
                         m[ 8 ] * m[ 2 ] * m[ 7 ] - m[ 8 ] * m[ 3 ] * m[ 6 ];

        result( 2, 3 ) = -m[ 0 ] * m[ 5 ] * m[ 11 ] + m[ 0 ] * m[ 7 ] * m[ 9 ] +
                         m[ 4 ] * m[ 1 ] * m[ 11 ] - m[ 4 ] * m[ 3 ] * m[ 9 ] -
                         m[ 8 ] * m[ 1 ] * m[ 7 ] + m[ 8 ] * m[ 3 ] * m[ 5 ];

        result( 3, 3 ) = m[ 0 ] * m[ 5 ] * m[ 10 ] - m[ 0 ] * m[ 6 ] * m[ 9 ] -
                         m[ 4 ] * m[ 1 ] * m[ 10 ] + m[ 4 ] * m[ 2 ] * m[ 9 ] +
                         m[ 8 ] * m[ 1 ] * m[ 6 ] - m[ 8 ] * m[ 2 ] * m[ 5 ];

        double det = m[ 0 ] * result[ 0 ] + m[ 1 ] * result[ 4 ] + m[ 2 ] * result[ 8 ] +
                     m[ 3 ] * result[ 12 ];

        if( abs( det ) < TType( 1e-7 ) )
        {
            throw std::runtime_error( "Not invertible" );
        }

        for( int i = 0; i < 16; ++i )
        {
            result[ i ] /= det;
        }

        return result;
    }

    template < typename TType1, typename TType2, size_t TRows1, size_t TCols1, size_t TCols2 >
    auto operator*( Matrix< TType1, TRows1, TCols1 > const& lhs,
                    Matrix< TType2, TCols1, TCols2 > const& rhs )
    {
        using ResultType = decltype( std::declval< TType1 >() * std::declval< TType2 >() );

        Matrix< ResultType, TRows1, TCols2 > result;

        for( size_t i = 0; i < TRows1; ++i )
        {
            for( size_t j = 0; j < TCols2; ++j )
            {
                result( i, j ) = 0;
                for( size_t k = 0; k < TCols1; ++k )
                {
                    result( i, j ) += lhs( i, k ) * rhs( k, j );
                }
            }
        }

        return result;
    }

    template < typename TType1, typename TType2, size_t TRows, size_t TCols, typename Enabled >
    auto operator*( TType1 lhs, Matrix< TType2, TRows, TCols > const& rhs )
    {
        using ResultType = decltype( std::declval< TType1 >() * std::declval< TType2 >() );

        Matrix< ResultType, TRows, TCols > result;

        for( size_t i = 0; i < TRows; ++i )
        {
            for( size_t j = 0; j < TCols; ++j )
            {
                result( i, j ) = lhs * rhs( i, j );
            }
        }

        return result;
    }

    template < typename TType1, typename TType2, size_t TRows, size_t TCols >
    auto operator+( Matrix< TType1, TRows, TCols > const& lhs,
                    Matrix< TType2, TRows, TCols > const& rhs )
    {
        using ResultType = decltype( std::declval< TType1 >() - std::declval< TType2 >() );

        Matrix< ResultType, TRows, TCols > result;

        for( size_t i = 0; i < TRows; ++i )
        {
            for( size_t j = 0; j < TCols; ++j )
            {
                result( i, j ) = lhs( i, j ) + rhs( i, j );
            }
        }

        return result;
    }

    template < typename TType1, typename TType2, size_t TRows, size_t TCols >
    auto operator-( Matrix< TType1, TRows, TCols > const& lhs,
                    Matrix< TType2, TRows, TCols > const& rhs )
    {
        using ResultType = decltype( std::declval< TType1 >() - std::declval< TType2 >() );

        Matrix< ResultType, TRows, TCols > result;

        for( size_t i = 0; i < TRows; ++i )
        {
            for( size_t j = 0; j < TCols; ++j )
            {
                result( i, j ) = lhs( i, j ) - rhs( i, j );
            }
        }

        return result;
    }

    template < typename TType1, typename TType2, size_t TRows, size_t TCols >
    Matrix< TType1, TRows, TCols >& operator*=( Matrix< TType1, TRows, TCols >& lhs,
                                                Matrix< TType2, TCols, TCols > const& rhs )
    {
        return lhs = lhs * rhs;
    }

    template < typename TType1, typename TType2, size_t TRows, size_t TCols >
    auto operator*( Matrix< TType1, TRows, TCols > const& matrix,
                    Vector< TType2, TCols > const& vector )
    {
        using ResultType = decltype( std::declval< TType1 >() * std::declval< TType2 >() );

        Vector< ResultType, TRows > result;

        for( size_t row = 0; row < TRows; ++row )
        {
            result( row ) = 0;
            for( size_t col = 0; col < TCols; ++col )
            {
                result( row ) += matrix( row, col ) * vector( col );
            }
        }

        return result;
    }

    template < typename TType1, typename TType2, size_t TDim >
    auto operator*( Matrix< TType1, TDim + 1, TDim + 1 > const& matrix,
                    Vector< TType2, TDim > const& vector )
    {
        return dehomogenize( matrix * homogenize( vector ) );
    }

    template < typename TType, size_t TRows, size_t TCols >
    std::ostream& operator<<( std::ostream& stream, Matrix< TType, TRows, TCols > const& mat )
    {
        stream << '(';
        for( size_t i = 0; i < mat.size; ++i )
        {
            if( i != 0 )
            {
                stream << ", ";
            }
            stream << mat[ i ];
        }
        stream << ')';

        return stream;
    }


    namespace mat
    {
        template < typename TType, size_t TDim >
        Matrix< TType, TDim, TDim > identity()
        {
            Matrix< TType, TDim, TDim > result;

            for( size_t r = 0; r < TDim; ++r )
            {
                for( size_t c = 0; c < TDim; ++c )
                {
                    result( r, c ) = ( r == c ) ? 1 : 0;
                }
            }

            return result;
        }

        template < typename TType, size_t TDim >
        Matrix< TType, TDim + 1, TDim + 1 > translation( Vector< TType, TDim > const& vec )
        {
            auto result = mat::identity< TType, TDim + 1 >();

            for( size_t d = 0; d < TDim; ++d )
            {
                result( d, TDim ) = vec( d );
            }

            return result;
        }

        template < typename TType >
        Matrix< TType, 3, 3 > rotation( TType angle )
        {
            using std::cos;
            using std::sin;

            auto const c = cos( angle );
            auto const s = sin( angle );

            // clang-format off
            return {
                c         , -s        , TType{ 0 },
                s         , c         , TType{ 0 },
                TType{ 0 }, TType{ 0 }, TType{ 1 }
            };
            // clang-format on
        }

        template < typename TType >
        Matrix< TType, 4, 4 > rotation( Quaternion< TType > const& quat )
        {
            TType const x2 = quat.x() * quat.x();
            TType const y2 = quat.y() * quat.y();
            TType const z2 = quat.z() * quat.z();
            TType const xy = quat.x() * quat.y();
            TType const xz = quat.x() * quat.z();
            TType const yz = quat.y() * quat.z();
            TType const wx = quat.w() * quat.x();
            TType const wy = quat.w() * quat.y();
            TType const wz = quat.w() * quat.z();

            // clang-format off
            return {
                TType( 1 ) - TType( 2 ) * ( y2 + z2 ), TType( 2 ) * ( xy - wz )             , TType( 2 ) * ( xz + wy )             , TType( 0 ),
                TType( 2 ) * ( xy + wz )             , TType( 1 ) - TType( 2 ) * ( x2 + z2 ), TType( 2 ) * ( yz - wx )             , TType( 0 ),
                TType( 2 ) * ( xz - wy )             , TType( 2 ) * ( yz + wx )             , TType( 1 ) - TType( 2 ) * ( x2 + y2 ), TType( 0 ),
                TType( 0 )                           , TType( 0 )                           , TType( 0 )                           , TType( 1 )
            };
            // clang-format on
        }

        template < typename TType, size_t TDim >
        Matrix< TType, TDim + 1, TDim + 1 > scale( Vector< TType, TDim > const& vec )
        {
            auto result = mat::identity< TType, TDim + 1 >();

            for( size_t d = 0; d < TDim; ++d )
            {
                result( d, d ) = vec( d );
            }

            return result;
        }


        template < typename TType, size_t TDim >
        Matrix< TType, TDim + 1, TDim + 1 > transform( Range< TType, TDim > const& from,
                                                       Range< TType, TDim > const& to )
        {
            auto result = mat::identity< TType, TDim + 1 >();

            for( size_t d = 0; d < TDim; ++d )
            {
                auto const factor = to.extent( d ) / from.extent( d );
                result( d, d ) = factor;
                result( d, TDim ) = to.min( d ) - factor * from.min( d );
            }

            return result;
        }


        template < typename TType >
        Matrix< TType, 4, 4 >
            ortho( TType right, TType top, TType left, TType bottom, TType zNear, TType zFar )
        {
            // clang-format off
            return {
                TType( 2 ) / ( right - left ), TType( 0 )                   , TType( 0 )                   , ( right + left ) / ( left - right ),
                TType( 0 )                   , TType( 2 ) / ( top - bottom ), TType( 0 )                   , ( top + bottom ) / ( bottom - top ),
                TType( 0 )                   , TType( 0 )                   , TType( 2 ) / ( zFar - zNear ), ( zFar + zNear ) / ( zNear - zFar ),
                TType( 0 )                   , TType( 0 )                   , TType( 0 )                   , TType( 1 )
            };
            // clang-format on
        }


        template < typename TType >
        Matrix< TType, 4, 4 > perspective( TType fovx, TType aspect, TType zNear, TType zFar )
        {
            using std::tan;
            fovx = TType( 1 ) / tan( fovx / TType( 2 ) );

            // clang-format off
            return {
                fovx      , TType( 0 )   , TType( 0 )                         , TType( 0 )                                  ,
                TType( 0 ), fovx * aspect, TType( 0 )                         , TType( 0 )                                  ,
                TType( 0 ), TType( 0 )   , ( zFar + zNear ) / ( zNear - zFar ), TType( 2 ) * zFar * zNear / ( zNear - zFar ),
                TType( 0 ), TType( 0 )   , TType( -1 )                        , TType( 0 )
            };
            // clang-format on
        }

        template < typename TType >
        Matrix< TType, 4, 4 > perspectiveInv( TType fovx, TType aspect, TType zNear, TType zFar )
        {
            using std::tan;
            fovx = TType( 1 ) / tan( fovx / TType( 2 ) );

            // clang-format off
            return {
                TType( 1 ) / fovx, TType( 0 )   , TType( 0 )                                  , TType( 0 )                                  ,
                TType( 0 )       , aspect / fovx, TType( 0 )                                  , TType( 0 )                                  ,
                TType( 0 )       , TType( 0 )   , TType( 0 )                                  , TType( -1 )                                 ,
                TType( 0 )       , TType( 0 )   , ( zNear - zFar ) / TType( 2 ) / zNear / zFar, ( zNear + zFar ) / TType( 2 ) / zNear / zFar
            };
            // clang-format on
        }

        template < typename TType >
        Matrix< TType, 4, 4 > lookAt( Vector< TType, 3 > const& from,
                                      Vector< TType, 3 > const& to,
                                      Vector< TType, 3 > const& up )
        {
            auto const z = normalize( from - to );
            auto const x = normalize( cross( up, z ) );
            auto const y = cross( z, x );

            // clang-format off
        return {
                x( 0 ),     x( 1 ),     x( 2 ), -dot( from, x ),
                y( 0 ),     y( 1 ),     y( 2 ), -dot( from, y ),
                z( 0 ),     z( 1 ),     z( 2 ), -dot( from, z ),
            TType( 0 ), TType( 0 ), TType( 0 ),      TType( 1 )
        };
            // clang-format on
        }

        template < typename TType >
        Matrix< TType, 4, 4 > lookAtInv( Vector< TType, 3 > const& from,
                                         Vector< TType, 3 > const& to,
                                         Vector< TType, 3 > const& up )
        {
            auto const z = normalize( from - to );
            auto const x = normalize( cross( up, z ) );
            auto const y = cross( z, x );

            // clang-format off
        return {
                     x( 0 ),          y( 0 ),          z( 0 ), TType( 0 ),
                     x( 1 ),          y( 1 ),          z( 1 ), TType( 0 ),
                     x( 2 ),          y( 2 ),          z( 2 ), TType( 0 ),
            -dot( from, x ), -dot( from, y ), -dot( from, z ), TType( 1 )
        };
            // clang-format on
        }
    }
}
