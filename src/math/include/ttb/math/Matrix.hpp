#pragma once

#include <ttb/math/Quaternion.hpp>
#include <ttb/math/Tensor.hpp>
#include <ttb/math/Vector.hpp>

#include <cstdlib>

// declarations
//=============================================================================

namespace ttb
{
    template < typename TType, size_t TDimension1, size_t TDimension2 >
    using Matrix = Tensor< TType, TDimension1, TDimension2 >;

    using Matrix4f = Matrix< float, 4, 4 >;



    template < typename T, size_t D1, size_t D2 >
    Matrix< T, D2, D1 > transpose( Matrix< T, D1, D2 > const& matrix );

    template < typename T >
    Matrix< T, 4, 4 > invert( Matrix< T, 4, 4 > const& matrix );

    template < typename T, size_t D1, size_t D2, size_t D3 >
    Matrix< T, D1, D3 > operator*( Matrix< T, D1, D2 > const& lhs, Matrix< T, D2, D3 > const& rhs );

    template < typename T, size_t D1, size_t D2, size_t D3 >
    Matrix< T, D1, D3 >& operator*=( Matrix< T, D1, D2 >& lhs, Matrix< T, D2, D3 > const& rhs );

    template < typename T, size_t D1, size_t D2 >
    Vector< T, D1 > operator*( Matrix< T, D1, D2 > const& matrix, Vector< T, D2 > const& vector );

    template < typename T >
    Vector< T, 3 > operator*( Matrix< T, 4, 4 > const& matrix, Vector< T, 3 > const& vector );



    template < typename T >
    class MatrixFactory
    {
    public:
        static Matrix< T, 4, 4 > identity();

        static Matrix< T, 4, 4 > ortho( T right, T top, T left, T bottom, T zNear, T zFar );

        static Matrix< T, 4, 4 > perspective( T fovy, T aspect, T zNear, T zFar );

        static Matrix< T, 4, 4 > perspectiveInv( T fovy, T aspect, T zNear, T zFar );

        static Matrix< T, 4, 4 > lookAt( Vector< T, 3 > const& eye, Vector< T, 3 > const& center,
                                         Vector< T, 3 > const& up );

        static Matrix< T, 4, 4 > lookAtInv( Vector< T, 3 > const& eye, Vector< T, 3 > const& center,
                                            Vector< T, 3 > const& up );

        static Matrix< T, 4, 4 > rotation( Quaternion< T > const& quat );

        static Matrix< T, 4, 4 > translation( Vector< T, 3 > const& vec );

        static Matrix< T, 4, 4 > scale( Vector< T, 3 > const& vec );

    private:
        MatrixFactory() = default;
    };
}



// definitions
//=================================================================================================

namespace ttb
{
    template < typename TType, size_t TDimension1, size_t TDimension2 >
    Matrix< TType, TDimension2, TDimension1 > transpose(
        Matrix< TType, TDimension1, TDimension2 > const& matrix )
    {
        Matrix< TType, TDimension2, TDimension1 > result;

        for( size_t row = 0; row < TDimension1; ++row )
            for( size_t col = 0; col < TDimension2; ++col )
                result( col, row ) = matrix( row, col );

        return result;
    }

    template < typename TType >
    Matrix< TType, 4, 4 > invert( Matrix< TType, 4, 4 > const& matrix )
    {
        using std::abs;

        Matrix< TType, 4, 4 > result;

        result( 0, 0 ) =
            matrix[ 5 ] * matrix[ 10 ] * matrix[ 15 ] - matrix[ 5 ] * matrix[ 11 ] * matrix[ 14 ] -
            matrix[ 9 ] * matrix[ 6 ] * matrix[ 15 ] + matrix[ 9 ] * matrix[ 7 ] * matrix[ 14 ] +
            matrix[ 13 ] * matrix[ 6 ] * matrix[ 11 ] - matrix[ 13 ] * matrix[ 7 ] * matrix[ 10 ];

        result( 1, 0 ) =
            -matrix[ 4 ] * matrix[ 10 ] * matrix[ 15 ] + matrix[ 4 ] * matrix[ 11 ] * matrix[ 14 ] +
            matrix[ 8 ] * matrix[ 6 ] * matrix[ 15 ] - matrix[ 8 ] * matrix[ 7 ] * matrix[ 14 ] -
            matrix[ 12 ] * matrix[ 6 ] * matrix[ 11 ] + matrix[ 12 ] * matrix[ 7 ] * matrix[ 10 ];

        result( 2, 0 ) =
            matrix[ 4 ] * matrix[ 9 ] * matrix[ 15 ] - matrix[ 4 ] * matrix[ 11 ] * matrix[ 13 ] -
            matrix[ 8 ] * matrix[ 5 ] * matrix[ 15 ] + matrix[ 8 ] * matrix[ 7 ] * matrix[ 13 ] +
            matrix[ 12 ] * matrix[ 5 ] * matrix[ 11 ] - matrix[ 12 ] * matrix[ 7 ] * matrix[ 9 ];

        result( 3, 0 ) =
            -matrix[ 4 ] * matrix[ 9 ] * matrix[ 14 ] + matrix[ 4 ] * matrix[ 10 ] * matrix[ 13 ] +
            matrix[ 8 ] * matrix[ 5 ] * matrix[ 14 ] - matrix[ 8 ] * matrix[ 6 ] * matrix[ 13 ] -
            matrix[ 12 ] * matrix[ 5 ] * matrix[ 10 ] + matrix[ 12 ] * matrix[ 6 ] * matrix[ 9 ];

        result( 0, 1 ) =
            -matrix[ 1 ] * matrix[ 10 ] * matrix[ 15 ] + matrix[ 1 ] * matrix[ 11 ] * matrix[ 14 ] +
            matrix[ 9 ] * matrix[ 2 ] * matrix[ 15 ] - matrix[ 9 ] * matrix[ 3 ] * matrix[ 14 ] -
            matrix[ 13 ] * matrix[ 2 ] * matrix[ 11 ] + matrix[ 13 ] * matrix[ 3 ] * matrix[ 10 ];

        result( 1, 1 ) =
            matrix[ 0 ] * matrix[ 10 ] * matrix[ 15 ] - matrix[ 0 ] * matrix[ 11 ] * matrix[ 14 ] -
            matrix[ 8 ] * matrix[ 2 ] * matrix[ 15 ] + matrix[ 8 ] * matrix[ 3 ] * matrix[ 14 ] +
            matrix[ 12 ] * matrix[ 2 ] * matrix[ 11 ] - matrix[ 12 ] * matrix[ 3 ] * matrix[ 10 ];

        result( 2, 1 ) =
            -matrix[ 0 ] * matrix[ 9 ] * matrix[ 15 ] + matrix[ 0 ] * matrix[ 11 ] * matrix[ 13 ] +
            matrix[ 8 ] * matrix[ 1 ] * matrix[ 15 ] - matrix[ 8 ] * matrix[ 3 ] * matrix[ 13 ] -
            matrix[ 12 ] * matrix[ 1 ] * matrix[ 11 ] + matrix[ 12 ] * matrix[ 3 ] * matrix[ 9 ];

        result( 3, 1 ) =
            matrix[ 0 ] * matrix[ 9 ] * matrix[ 14 ] - matrix[ 0 ] * matrix[ 10 ] * matrix[ 13 ] -
            matrix[ 8 ] * matrix[ 1 ] * matrix[ 14 ] + matrix[ 8 ] * matrix[ 2 ] * matrix[ 13 ] +
            matrix[ 12 ] * matrix[ 1 ] * matrix[ 10 ] - matrix[ 12 ] * matrix[ 2 ] * matrix[ 9 ];

        result( 0, 2 ) =
            matrix[ 1 ] * matrix[ 6 ] * matrix[ 15 ] - matrix[ 1 ] * matrix[ 7 ] * matrix[ 14 ] -
            matrix[ 5 ] * matrix[ 2 ] * matrix[ 15 ] + matrix[ 5 ] * matrix[ 3 ] * matrix[ 14 ] +
            matrix[ 13 ] * matrix[ 2 ] * matrix[ 7 ] - matrix[ 13 ] * matrix[ 3 ] * matrix[ 6 ];

        result( 1, 2 ) =
            -matrix[ 0 ] * matrix[ 6 ] * matrix[ 15 ] + matrix[ 0 ] * matrix[ 7 ] * matrix[ 14 ] +
            matrix[ 4 ] * matrix[ 2 ] * matrix[ 15 ] - matrix[ 4 ] * matrix[ 3 ] * matrix[ 14 ] -
            matrix[ 12 ] * matrix[ 2 ] * matrix[ 7 ] + matrix[ 12 ] * matrix[ 3 ] * matrix[ 6 ];

        result( 2, 2 ) =
            matrix[ 0 ] * matrix[ 5 ] * matrix[ 15 ] - matrix[ 0 ] * matrix[ 7 ] * matrix[ 13 ] -
            matrix[ 4 ] * matrix[ 1 ] * matrix[ 15 ] + matrix[ 4 ] * matrix[ 3 ] * matrix[ 13 ] +
            matrix[ 12 ] * matrix[ 1 ] * matrix[ 7 ] - matrix[ 12 ] * matrix[ 3 ] * matrix[ 5 ];

        result( 3, 2 ) =
            -matrix[ 0 ] * matrix[ 5 ] * matrix[ 14 ] + matrix[ 0 ] * matrix[ 6 ] * matrix[ 13 ] +
            matrix[ 4 ] * matrix[ 1 ] * matrix[ 14 ] - matrix[ 4 ] * matrix[ 2 ] * matrix[ 13 ] -
            matrix[ 12 ] * matrix[ 1 ] * matrix[ 6 ] + matrix[ 12 ] * matrix[ 2 ] * matrix[ 5 ];

        result( 0, 3 ) =
            -matrix[ 1 ] * matrix[ 6 ] * matrix[ 11 ] + matrix[ 1 ] * matrix[ 7 ] * matrix[ 10 ] +
            matrix[ 5 ] * matrix[ 2 ] * matrix[ 11 ] - matrix[ 5 ] * matrix[ 3 ] * matrix[ 10 ] -
            matrix[ 9 ] * matrix[ 2 ] * matrix[ 7 ] + matrix[ 9 ] * matrix[ 3 ] * matrix[ 6 ];

        result( 1, 3 ) =
            matrix[ 0 ] * matrix[ 6 ] * matrix[ 11 ] - matrix[ 0 ] * matrix[ 7 ] * matrix[ 10 ] -
            matrix[ 4 ] * matrix[ 2 ] * matrix[ 11 ] + matrix[ 4 ] * matrix[ 3 ] * matrix[ 10 ] +
            matrix[ 8 ] * matrix[ 2 ] * matrix[ 7 ] - matrix[ 8 ] * matrix[ 3 ] * matrix[ 6 ];

        result( 2, 3 ) =
            -matrix[ 0 ] * matrix[ 5 ] * matrix[ 11 ] + matrix[ 0 ] * matrix[ 7 ] * matrix[ 9 ] +
            matrix[ 4 ] * matrix[ 1 ] * matrix[ 11 ] - matrix[ 4 ] * matrix[ 3 ] * matrix[ 9 ] -
            matrix[ 8 ] * matrix[ 1 ] * matrix[ 7 ] + matrix[ 8 ] * matrix[ 3 ] * matrix[ 5 ];

        result( 3, 3 ) =
            matrix[ 0 ] * matrix[ 5 ] * matrix[ 10 ] - matrix[ 0 ] * matrix[ 6 ] * matrix[ 9 ] -
            matrix[ 4 ] * matrix[ 1 ] * matrix[ 10 ] + matrix[ 4 ] * matrix[ 2 ] * matrix[ 9 ] +
            matrix[ 8 ] * matrix[ 1 ] * matrix[ 6 ] - matrix[ 8 ] * matrix[ 2 ] * matrix[ 5 ];

        double det = matrix[ 0 ] * result[ 0 ] + matrix[ 1 ] * result[ 4 ] +
                     matrix[ 2 ] * result[ 8 ] + matrix[ 3 ] * result[ 12 ];

        if( abs( det ) < TType( 1e-7 ) )
            throw std::runtime_error( "Not invertible" );

        for( int i = 0; i < 16; i++ )
            result[ i ] /= det;

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

    template < typename T, size_t D1, size_t D2, size_t D3 >
    Matrix< T, D1, D3 >& operator*=( Matrix< T, D1, D2 >& lhs, Matrix< T, D2, D3 > const& rhs )
    {
        return lhs = lhs * rhs;
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

    template < typename T >
    Vector< T, 3 > operator*( Matrix< T, 4, 4 > const& matrix, Vector< T, 3 > const& vector )
    {
        auto vec = matrix * Vector< T, 4 >{ vector( 0 ), vector( 1 ), vector( 2 ), 1.0 };
        return { vec( 0 ) / vec( 3 ), vec( 1 ) / vec( 3 ), vec( 2 ) / vec( 3 ) };
    }



    template < typename T >
    Matrix< T, 4, 4 > MatrixFactory< T >::identity()
    {
        return { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    }

    template < typename TType >
    Matrix< TType, 4, 4 > MatrixFactory< TType >::ortho( TType right, TType top, TType left,
                                                         TType bottom, TType zNear, TType zFar )
    {
        return { TType( 2 ) / ( right - left ),
                 TType( 0 ),
                 TType( 0 ),
                 ( right + left ) / ( left - right ),
                 TType( 0 ),
                 TType( 2 ) / ( top - bottom ),
                 TType( 0 ),
                 ( top + bottom ) / ( bottom - top ),
                 TType( 0 ),
                 TType( 0 ),
                 TType( 2 ) / ( zFar - zNear ),
                 ( zFar + zNear ) / ( zNear - zFar ),
                 TType( 0 ),
                 TType( 0 ),
                 TType( 0 ),
                 TType( 1 ) };
    }

    template < typename TType >
    Matrix< TType, 4, 4 > MatrixFactory< TType >::perspective( TType fovy, TType aspect,
                                                               TType zNear, TType zFar )
    {
        using std::tan;
        fovy = TType( 1 ) / tan( fovy );

        return { fovy / aspect,
                 TType( 0 ),
                 TType( 0 ),
                 TType( 0 ),
                 TType( 0 ),
                 fovy,
                 TType( 0 ),
                 TType( 0 ),
                 TType( 0 ),
                 TType( 0 ),
                 ( zFar + zNear ) / ( zNear - zFar ),
                 TType( 2 ) * zFar * zNear / ( zNear - zFar ),
                 TType( 0 ),
                 TType( 0 ),
                 TType( -1 ),
                 TType( 0 ) };
    }

    template < typename TType >
    Matrix< TType, 4, 4 > MatrixFactory< TType >::perspectiveInv( TType fovy, TType aspect,
                                                                  TType zNear, TType zFar )
    {
        using std::tan;

        fovy = TType( 1 ) / tan( fovy );

        return { aspect / fovy,
                 TType( 0 ),
                 TType( 0 ),
                 TType( 0 ),
                 TType( 0 ),
                 TType( 1 ) / fovy,
                 TType( 0 ),
                 TType( 0 ),
                 TType( 0 ),
                 TType( 0 ),
                 TType( 0 ),
                 TType( -1 ),
                 TType( 0 ),
                 TType( 0 ),
                 ( zNear - zFar ) / TType( 2 ) / zNear / zFar,
                 ( zNear + zFar ) / TType( 2 ) / zNear / zFar };
    }

    template < typename TType >
    Matrix< TType, 4, 4 > MatrixFactory< TType >::lookAt( Vector< TType, 3 > const& eye,
                                                          Vector< TType, 3 > const& center,
                                                          Vector< TType, 3 > const& up )
    {
        auto F = normalize( center - eye );
        auto s = normalize( cross( F, up ) );
        auto u = cross( s, F );

        return { s( 0 ),     s( 1 ),         s( 2 ),     -dot( eye, s ), u( 0 ),  u( 1 ),
                 u( 2 ),     -dot( eye, u ), -F( 0 ),    -F( 1 ),        -F( 2 ), dot( eye, F ),
                 TType( 0 ), TType( 0 ),     TType( 0 ), TType( 1 ) };
    }

    template < typename TType >
    Matrix< TType, 4, 4 > MatrixFactory< TType >::lookAtInv( Vector< TType, 3 > const& eye,
                                                             Vector< TType, 3 > const& center,
                                                             Vector< TType, 3 > const& up )
    {
        auto F = normalize( center - eye );
        auto s = normalize( cross( F, up ) );
        auto u = cross( s, F );

        return {
            s( 0 ), u( 0 ), -F( 0 ), eye( 0 ), s( 1 ),     u( 1 ),     -F( 1 ),    eye( 1 ),
            s( 2 ), u( 2 ), -F( 2 ), eye( 2 ), TType( 0 ), TType( 0 ), TType( 0 ), TType( 1 )
        };
    }

    template < typename TType >
    Matrix< TType, 4, 4 > MatrixFactory< TType >::rotation( Quaternion< TType > const& quat )
    {
        TType x2 = quat.x() * quat.x();
        TType y2 = quat.y() * quat.y();
        TType z2 = quat.z() * quat.z();
        TType xy = quat.x() * quat.y();
        TType xz = quat.x() * quat.z();
        TType yz = quat.y() * quat.z();
        TType wx = quat.w() * quat.x();
        TType wy = quat.w() * quat.y();
        TType wz = quat.w() * quat.z();

        return { TType( 1 ) - TType( 2 ) * ( y2 + z2 ),
                 TType( 2 ) * ( xy - wz ),
                 TType( 2 ) * ( xz + wy ),
                 TType( 0 ),
                 TType( 2 ) * ( xy + wz ),
                 TType( 1 ) - TType( 2 ) * ( x2 + z2 ),
                 TType( 2 ) * ( yz - wx ),
                 TType( 0 ),
                 TType( 2 ) * ( xz - wy ),
                 TType( 2 ) * ( yz + wx ),
                 TType( 1 ) - TType( 2 ) * ( x2 + y2 ),
                 TType( 0 ),
                 TType( 0 ),
                 TType( 0 ),
                 TType( 0 ),
                 TType( 1 ) };
    }

    template < typename TType >
    Matrix< TType, 4, 4 > MatrixFactory< TType >::translation( Vector< TType, 3 > const& vec )
    {
        return { TType( 1 ), TType( 0 ), TType( 0 ), vec( 0 ),   TType( 0 ), TType( 1 ),
                 TType( 0 ), vec( 1 ),   TType( 0 ), TType( 0 ), TType( 1 ), vec( 2 ),
                 TType( 0 ), TType( 0 ), TType( 0 ), TType( 1 ) };
    }

    template < typename TType >
    Matrix< TType, 4, 4 > MatrixFactory< TType >::scale( Vector< TType, 3 > const& vec )
    {
        return { vec( 0 ),   TType( 0 ), TType( 0 ), TType( 0 ), TType( 0 ), vec( 1 ),
                 TType( 0 ), TType( 0 ), TType( 0 ), TType( 0 ), vec( 2 ),   TType( 0 ),
                 TType( 0 ), TType( 0 ), TType( 0 ), TType( 1 ) };
    }
}
