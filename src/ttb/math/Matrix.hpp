#pragma once

#include <ttb/math/Quaternion.hpp>
#include <ttb/math/Tensor.hpp>
#include <ttb/math/Vector.hpp>

#include <cstdlib>

// declarations
//=============================================================================

namespace ttb
{
    template < typename T, size_t D1, size_t D2 >
    using Matrix = Tensor< T, D1, D2 >;

    using Matrix4f = Matrix< float, 4, 4 >;



    template < typename T, size_t D1, size_t D2 >
    Matrix< T, D2, D1 > transpose( Matrix< T, D1, D2 > const& matrix );

    template < typename T >
    Matrix< T, 4, 4 > invert( Matrix< T, 4, 4 > const& matrix );

    template < typename T, size_t D1, size_t D2, size_t D3 >
    Matrix< T, D1, D3 > operator*( Matrix< T, D1, D2 > const& lhs, Matrix< T, D2, D3 > const& rhs );

    template < typename T, size_t D1, size_t D2 >
    Vector< T, D1 > operator*( Matrix< T, D1, D2 > const& matrix, Vector< T, D2 > const& vector );

    template < typename T >
    Vector< T, 3 > operator*( Matrix< T, 4, 4 > const& matrix, Vector< T, 3 > const& vector );



    template < typename T >
    class MatrixFactory
    {
    public:
        static Matrix< T, 4, 4 > fromIdentity();

        static Matrix< T, 4, 4 > fromOrtho( T right, T top, T left, T bottom, T zNear, T zFar );

        static Matrix< T, 4, 4 > fromPerspective( T fovy, T aspect, T zNear, T zFar );

        static Matrix< T, 4, 4 > fromPerspectiveInv( T fovy, T aspect, T zNear, T zFar );

        static Matrix< T, 4, 4 > fromLookAt( Vector< T, 3 > const& eye,
                                             Vector< T, 3 > const& center,
                                             Vector< T, 3 > const& up );

        static Matrix< T, 4, 4 > fromLookAtInv( Vector< T, 3 > const& eye,
                                                Vector< T, 3 > const& center,
                                                Vector< T, 3 > const& up );

        static Matrix< T, 4, 4 > fromRotation( Quaternion< T > const& quat );

        static Matrix< T, 4, 4 > fromTranslation( Vector< T, 3 > const& vec );

        static Matrix< T, 4, 4 > fromScale( Vector< T, 3 > const& vec );

    private:
        MatrixFactory() = default;
    };
}



// definitions
//=================================================================================================

namespace ttb
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

    template < typename T >
    Matrix< T, 4, 4 > invert( Matrix< T, 4, 4 > const& matrix )
    {
        Matrix< T, 4, 4 > result;

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

        if( std::abs( det ) < 1e-7 )
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
    Matrix< T, 4, 4 > MatrixFactory< T >::fromIdentity()
    {
        return { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 };
    }

    template < typename T >
    Matrix< T, 4, 4 > MatrixFactory< T >::fromOrtho( T right, T top, T left, T bottom, T zNear,
                                                     T zFar )
    {
        return { 2.0 / ( right - left ),
                 0.0,
                 0.0,
                 ( right + left ) / ( left - right ),
                 0.0,
                 2.0 / ( top - bottom ),
                 0.0,
                 ( top + bottom ) / ( bottom - top ),
                 0.0,
                 0.0,
                 2.0 / ( zFar - zNear ),
                 ( zFar + zNear ) / ( zNear - zFar ),
                 0.0,
                 0.0,
                 0.0,
                 1.0 };
    }

    template < typename T >
    Matrix< T, 4, 4 > MatrixFactory< T >::fromPerspective( T fovy, T aspect, T zNear, T zFar )
    {
        fovy = 1.0 / std::tan( fovy / 114.5915590261646417536 );

        return { fovy / aspect,
                 0.0f,
                 0.0f,
                 0.0f,
                 0.0f,
                 fovy,
                 0.0f,
                 0.0f,
                 0.0f,
                 0.0f,
                 ( zFar + zNear ) / ( zNear - zFar ),
                 2.0f * zFar * zNear / ( zNear - zFar ),
                 0.0f,
                 0.0f,
                 -1.0f,
                 0.0f };
    }

    template < typename T >
    Matrix< T, 4, 4 > MatrixFactory< T >::fromPerspectiveInv( T fovy, T aspect, T zNear, T zFar )
    {
        fovy = 1.0 / std::tan( fovy / 114.5915590261646417536 );

        return { aspect / fovy,
                 0.0f,
                 0.0f,
                 0.0f,
                 0.0f,
                 1.0f / fovy,
                 0.0f,
                 0.0f,
                 0.0f,
                 0.0f,
                 0.0f,
                 -1.0f,
                 0.0f,
                 0.0f,
                 ( zNear - zFar ) / 2.0f / zNear / zFar,
                 ( zNear + zFar ) / 2.0f / zNear / zFar };
    }

    template < typename T >
    Matrix< T, 4, 4 > MatrixFactory< T >::fromLookAt( Vector< T, 3 > const& eye,
                                                      Vector< T, 3 > const& center,
                                                      Vector< T, 3 > const& up )
    {
        auto F = normalize( center - eye );
        auto s = normalize( cross( F, up ) );
        auto u = cross( s, F );

        return { s( 0 ),  s( 1 ),  s( 2 ),  -dot( eye, s ), u( 0 ), u( 1 ), u( 2 ), -dot( eye, u ),
                 -F( 0 ), -F( 1 ), -F( 2 ), dot( eye, F ),  0.0,    0.0,    0.0,    1.0 };
    }

    template < typename T >
    Matrix< T, 4, 4 > MatrixFactory< T >::fromLookAtInv( Vector< T, 3 > const& eye,
                                                         Vector< T, 3 > const& center,
                                                         Vector< T, 3 > const& up )
    {
        auto F = normalize( center - eye );
        auto s = normalize( cross( F, up ) );
        auto u = cross( s, F );

        return { s( 0 ), u( 0 ), -F( 0 ), eye( 0 ), s( 1 ), u( 1 ), -F( 1 ), eye( 1 ),
                 s( 2 ), u( 2 ), -F( 2 ), eye( 2 ), 0.0,    0.0,    0.0,     1.0 };
    }

    template < typename T >
    Matrix< T, 4, 4 > MatrixFactory< T >::fromRotation( Quaternion< T > const& quat )
    {
        T x2 = quat.x() * quat.x();
        T y2 = quat.y() * quat.y();
        T z2 = quat.z() * quat.z();
        T xy = quat.x() * quat.y();
        T xz = quat.x() * quat.z();
        T yz = quat.y() * quat.z();
        T wx = quat.w() * quat.x();
        T wy = quat.w() * quat.y();
        T wz = quat.w() * quat.z();

        return { T( 1 ) - T( 2 ) * ( y2 + z2 ),
                 T( 2 ) * ( xy - wz ),
                 T( 2 ) * ( xz + wy ),
                 T( 0 ),
                 T( 2 ) * ( xy + wz ),
                 T( 1 ) - T( 2 ) * ( x2 + z2 ),
                 T( 2 ) * ( yz - wx ),
                 T( 0 ),
                 T( 2 ) * ( xz - wy ),
                 T( 2 ) * ( yz + wx ),
                 T( 1 ) - T( 2 ) * ( x2 + y2 ),
                 T( 0 ),
                 T( 0 ),
                 T( 0 ),
                 T( 0 ),
                 T( 1 ) };
    }

    template < typename T >
    Matrix< T, 4, 4 > MatrixFactory< T >::fromTranslation( Vector< T, 3 > const& vec )
    {
        return { T( 1 ), T( 0 ), T( 0 ), vec( 0 ), T( 0 ), T( 1 ), T( 0 ), vec( 1 ),
                 T( 0 ), T( 0 ), T( 1 ), vec( 2 ), T( 0 ), T( 0 ), T( 0 ), T( 1 ) };
    }

    template < typename T >
    Matrix< T, 4, 4 > MatrixFactory< T >::fromScale( Vector< T, 3 > const& vec )
    {
        return { vec( 0 ), T( 0 ), T( 0 ),   T( 0 ), T( 0 ), vec( 1 ), T( 0 ), T( 0 ),
                 T( 0 ),   T( 0 ), vec( 2 ), T( 0 ), T( 0 ), T( 0 ),   T( 0 ), T( 1 ) };
    }
}
