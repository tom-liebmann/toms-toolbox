#pragma once

#include <ttb/math/Vector.hpp>

#include <cmath>
#include <ostream>


namespace ttb
{
    namespace impl
    {
        template < typename TType >
        constexpr TType LENGTH_THRESHOLD = static_cast< TType >( 1e-8 );
    }
}


// declarations
//=============================================================================

namespace ttb
{
    template < typename TType >
    class Quaternion
    {
    public:
        Quaternion();
        Quaternion( TType w, TType x, TType y, TType z );
        Quaternion( TType rot, Vector< TType, 3 > const& axis );
        Quaternion( Vector< TType, 3 > const& from, Vector< TType, 3 > const& to );

        TType w() const;
        TType x() const;
        TType y() const;
        TType z() const;

        TType& w();
        TType& x();
        TType& y();
        TType& z();

        TType azimuth() const;
        TType polar() const;

    private:
        TType m_w;
        TType m_x;
        TType m_y;
        TType m_z;
    };

    template < typename TType >
    TType norm( Quaternion< TType > const& quat );

    template < typename TType >
    TType norm2( Quaternion< TType > const& quat );

    template < typename TType >
    TType dot( Quaternion< TType > const& lhs, Quaternion< TType > const& rhs );

    template < typename TType >
    Quaternion< TType > invert( Quaternion< TType > const& quat );

    template < typename TType >
    Quaternion< TType > normalize( Quaternion< TType > const& quat );

    template < typename TType >
    Quaternion< TType > exp( Quaternion< TType > const& quat );

    template < typename TType >
    Quaternion< TType > ln( Quaternion< TType > const& quat );

    template < typename TType >
    Quaternion< TType > pow( Quaternion< TType > const& quat, TType exponent );

    template < typename TType >
    Quaternion< TType >
        slerp( Quaternion< TType > const& q1, Quaternion< TType > const& q2, TType factor );

    template < typename TType >
    Quaternion< TType > operator*( Quaternion< TType > const& lhs, Quaternion< TType > const& rhs );

    template < typename TType >
    Quaternion< TType > operator*( TType lhs, Quaternion< TType > const& rhs );

    template < typename TType >
    Quaternion< TType > operator*( Quaternion< TType > const& lhs, TType rhs );

    template < typename TType >
    Quaternion< TType > operator+( Quaternion< TType > const& lhs, Quaternion< TType > const& rhs );

    template < typename TType >
    Vector< TType, 3 > operator*( Quaternion< TType > const& lhs, Vector< TType, 3 > const& rhs );

    template < typename TType >
    Quaternion< TType > operator-( Quaternion< TType > const& q );

    template < typename TType >
    std::ostream& operator<<( std::ostream& stream, Quaternion< TType > const& quat );
}



// definitions
//=============================================================================

namespace ttb
{
    template < typename TType >
    inline Quaternion< TType >::Quaternion() : m_w{ 1 }, m_x{ 0 }, m_y{ 0 }, m_z{ 0 }
    {
    }

    template < typename TType >
    inline Quaternion< TType >::Quaternion( TType w, TType x, TType y, TType z )
        : m_w{ w }, m_x{ x }, m_y{ y }, m_z{ z }
    {
    }

    template < typename TType >
    Quaternion< TType >::Quaternion( TType rot, Vector< TType, 3 > const& axis )
    {
        using std::sin;
        rot /= 2.0;
        auto const factor = sin( rot ) / norm( axis );
        m_w = std::cos( rot );
        m_x = axis.x() * factor;
        m_y = axis.y() * factor;
        m_z = axis.z() * factor;
    }

    template < typename TType >
    Quaternion< TType >::Quaternion( Vector< TType, 3 > const& from, Vector< TType, 3 > const& to )
    {
        Vector< TType, 3 > halfVec = from + to;
        halfVec = halfVec / norm( halfVec );
        m_w = dot( halfVec, to );
        halfVec = cross( halfVec, to );
        m_x = halfVec.x();
        m_y = halfVec.y();
        m_z = halfVec.z();
    }

    template < typename TType >
    inline TType Quaternion< TType >::w() const
    {
        return m_w;
    }

    template < typename TType >
    inline TType Quaternion< TType >::x() const
    {
        return m_x;
    }

    template < typename TType >
    inline TType Quaternion< TType >::y() const
    {
        return m_y;
    }

    template < typename TType >
    inline TType Quaternion< TType >::z() const
    {
        return m_z;
    }

    template < typename TType >
    inline TType& Quaternion< TType >::w()
    {
        return m_w;
    }

    template < typename TType >
    inline TType& Quaternion< TType >::x()
    {
        return m_x;
    }

    template < typename TType >
    inline TType& Quaternion< TType >::y()
    {
        return m_y;
    }

    template < typename TType >
    inline TType& Quaternion< TType >::z()
    {
        return m_z;
    }

    template < class Type >
    inline Type Quaternion< Type >::azimuth() const
    {
        return static_cast< Type >( atan2( m_y, m_x ) * 2.0 );
    }

    template < class Type >
    inline Type Quaternion< Type >::polar() const
    {
        return static_cast< Type >( atan2( m_y, m_z ) * 2.0 );
    }

    template < typename TType >
    inline TType norm( Quaternion< TType > const& quat )
    {
        using std::sqrt;
        return sqrt( norm2( quat ) );
    }

    template < typename TType >
    inline TType norm2( Quaternion< TType > const& quat )
    {
        return dot( quat, quat );
    }

    template < typename TType >
    inline TType dot( Quaternion< TType > const& lhs, Quaternion< TType > const& rhs )
    {
        return lhs.w() * rhs.w() + lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
    }

    template < typename TType >
    inline Quaternion< TType > invert( Quaternion< TType > const& quat )
    {
        return { quat.w(), -quat.x(), -quat.y(), -quat.z() };
    }

    template < typename TType >
    inline Quaternion< TType > normalize( Quaternion< TType > const& quat )
    {
        TType const len = norm( quat );
        return { quat.w() / len, quat.x() / len, quat.y() / len, quat.z() / len };
    }

    template < typename TType >
    Quaternion< TType > exp( Quaternion< TType > const& quat )
    {
        using namespace impl;
        using std::abs;
        using std::cos;
        using std::exp;
        using std::sin;
        using std::sqrt;

        TType expW = exp( quat.w() );
        TType len = sqrt( quat.x() * quat.x() + quat.y() * quat.y() + quat.z() * quat.z() );
        TType f = abs( len ) > LENGTH_THRESHOLD< TType > ? expW * sin( len ) / len
                                                         : static_cast< TType >( 0 );

        return { expW * cos( len ), quat.x() * f, quat.y() * f, quat.z() * f };
    }

    template < typename TType >
    Quaternion< TType > ln( Quaternion< TType > const& quat )
    {
        using namespace impl;
        using std::abs;
        using std::atan2;
        using std::log;
        using std::sqrt;

        auto const lenSq = quat.x() * quat.x() + quat.y() * quat.y() + quat.z() * quat.z();
        auto const len = sqrt( lenSq );
        auto const f = abs( len ) > LENGTH_THRESHOLD< TType > ? atan2( len, quat.w() ) / len
                                                              : static_cast< TType >( 0 );

        return { static_cast< TType >( 0.5 ) * log( quat.w() * quat.w() + lenSq ),
                 quat.x() * f,
                 quat.y() * f,
                 quat.z() * f };
    }

    template < typename TType >
    inline Quaternion< TType > pow( Quaternion< TType > const& quat, TType exponent )
    {
        return exp( exponent * ln( quat ) );
    }

    template < typename TType >
    inline Quaternion< TType >
        slerp( Quaternion< TType > const& q1, Quaternion< TType > const& q2, TType factor )
    {
        if( dot( q1, q2 ) < 0.0 )
        {
            return q1 * pow( invert( q1 ) * -q2, factor );
        }
        else
        {
            return q1 * pow( invert( q1 ) * q2, factor );
        }
    }

    template < typename TType >
    inline Quaternion< TType > operator*( Quaternion< TType > const& lhs,
                                          Quaternion< TType > const& rhs )
    {
        return { lhs.w() * rhs.w() - lhs.x() * rhs.x() - lhs.y() * rhs.y() - lhs.z() * rhs.z(),
                 lhs.w() * rhs.x() + lhs.x() * rhs.w() + lhs.y() * rhs.z() - lhs.z() * rhs.y(),
                 lhs.w() * rhs.y() + lhs.y() * rhs.w() + lhs.z() * rhs.x() - lhs.x() * rhs.z(),
                 lhs.w() * rhs.z() + lhs.z() * rhs.w() + lhs.x() * rhs.y() - lhs.y() * rhs.x() };
    }

    template < typename TType >
    inline Quaternion< TType > operator*( TType scalar, Quaternion< TType > const& quat )
    {
        return { quat.w() * scalar, quat.x() * scalar, quat.y() * scalar, quat.z() * scalar };
    }

    template < typename TType >
    inline Quaternion< TType > operator*( Quaternion< TType > const& quat, TType scalar )
    {
        return { quat.w() * scalar, quat.x() * scalar, quat.y() * scalar, quat.z() * scalar };
    }

    template < typename TType >
    inline Quaternion< TType > operator+( Quaternion< TType > const& lhs,
                                          Quaternion< TType > const& rhs )
    {
        return { lhs.w() + rhs.w(), lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z() };
    }

    template < typename TType >
    Vector< TType, 3 > operator*( Quaternion< TType > const& lhs, Vector< TType, 3 > const& rhs )
    {
        auto const v = Vector< TType, 3 >{ lhs.x(), lhs.y(), lhs.z() };
        auto const c = static_cast< TType >( 2.0 ) * cross( v, rhs );
        return rhs + lhs.w() * c + cross( v, c );
    }

    template < typename TType >
    inline Quaternion< TType > operator-( Quaternion< TType > const& q )
    {
        return { -q.w(), -q.x(), -q.y(), -q.z() };
    }

    template < typename TType >
    std::ostream& operator<<( std::ostream& stream, Quaternion< TType > const& quat )
    {
        stream << "( " << quat.w() << ", " << quat.x() << ", " << quat.y() << ", " << quat.z()
               << " )";

        return stream;
    }
}
