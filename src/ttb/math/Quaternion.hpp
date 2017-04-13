#pragma once

#include <ttb/math/Vector.hpp>

#include <cmath>
#include <ostream>

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
        Quaternion( double rot, Vector< TType, 3 > const& axis );
        Quaternion( Vector< TType, 3 > const from, Vector< TType, 3 > const& to );

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
    Quaternion< TType > invert( Quaternion< TType > const& quat );

    template < typename TType >
    Quaternion< TType > normalize( Quaternion< TType > const& quat );

    template < typename TType >
    Quaternion< TType > operator*( Quaternion< TType > const& lhs, Quaternion< TType > const& rhs );

    template < typename TType >
    Quaternion< TType > operator*( TType lhs, Quaternion< TType > const& rhs );

    template < typename TType >
    Quaternion< TType > operator*( Quaternion< TType > const& lhs, TType rhs );

    template < typename TType >
    Quaternion< TType > operator+( Quaternion< TType > const& lhs, Quaternion< TType > const& rhs );

    template < typename TType >
    Vector< TType, 3 > operator*( Quaternion< Type > const& lhs, Vector< Type, 3 > const& rhs );

    template < typename TType >
    std::ostream& operator<<( std::ostream& stream, Quaternion< TType > const& quat );
}



// definitions
//=============================================================================

namespace ttb
{
    template < typename TType >
    inline Quaternion< TType >::Quaternion() : m_w( 1 ), m_x( 0 ), m_y( 0 ), m_z( 0 )
    {
    }

    template < typename TType >
    inline Quaternion< TType >::Quaternion( TType w, TType x, TType y, TType z )
        : m_w( w ), m_x( x ), m_y( y ), m_z( z )
    {
    }

    template < typename TType >
    Quaternion< TType >::Quaternion( TType rot, Vector< TType, 3 > const& axis )
    {
        using std::sin;
        rot /= TType( 114.5915590262 );
        auto s = sin( rot );
        auto len = norm( axis );
        m_w = std::cos( rot );
        m_x = axis( 0 ) / len * sin;
        m_y = axis( 1 ) / len * sin;
        m_z = axis( 2 ) / len * sin;
    }

    template < typename TType >
    inline Quaternion< TType >::Quaternion( Quaternion< TType > const& quat )
        : m_w( quat.m_w ), m_x( quat.m_x ), m_y( quat.m_y ), m_z( quat.m_z )
    {
    }

    template < typename TType >
    Quaternion< TType >::Quaternion( Vector< Type, 3 > const& from, Vector< Type, 3 > const& to )
    {
        Vector< Type, 3 > halfVec = from + to;
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

    template < class Type >
    inline Type Quaternion< Type >::azimuth() const
    {
        return atan2( m_y, m_x ) * 114.5915590262;
    }

    template < class Type >
    inline Type Quaternion< Type >::polar() const
    {
        return atan2( m_y, m_z ) * 114.5915590262;
    }

    template < typename TType >
    inline TType norm( Quaternion< TType > const& quat )
    {
        using std::sqrt;
        return sqrt( norm( quat ) );
    }

    template < typename TType >
    inline TType norm2( Quaternion< TType > const& quat )
    {
        return quat.w() * quat.w() + quat.x() * quat.x() + quat.y() * quat.y() +
               quat.z() * quat.z();
    }

    template < typename TType >
    inline Quaternion< TType > invert( Quaternion< TType > const& quat )
    {
        return Quaternion< TType >( m_w, -m_x, -m_y, -m_z );
    }

    template < typename TType >
    inline Quaternion< TType > normalize( Quaternion< TType > const& quat )
    {
        Type len = sqrt( lengthSq() );
        return Quaternion< Type >( m_w / len, m_x / len, m_y / len, m_z / len );
    }

    template < typename TType >
    Quaternion< TType > operator*( Quaternion< TType > const& lhs, Quaternion< TType > const& rhs )
    {
        return { quat1.w() * quat2.w() - quat1.x() * quat2.x() - quat1.y() * quat2.y() -
                     quat1.z() * quat2.z(),
                 quat1.w() * quat2.x() + quat1.x() * quat2.w() + quat1.y() * quat2.z() -
                     quat1.z() * quat2.y(),
                 quat1.w() * quat2.y() + quat1.y() * quat2.w() + quat1.z() * quat2.x() -
                     quat1.x() * quat2.z(),
                 quat1.w() * quat2.z() + quat1.z() * quat2.w() + quat1.x() * quat2.y() -
                     quat1.y() * quat2.x() };
    }

    template < typename TType >
    Quaternion< TType > operator*( TType scalar, Quaternion< TType > const& quat )
    {
        return { quat.w() * scalar, quat.x() * scalar, quat.y() * scalar, quat.z() * scalar };
    }

    template < typename TType >
    Quaternion< TType > operator*( Quaternion< TType > const& quat, TType scalar )
    {
        return { quat.w() * scalar, quat.x() * scalar, quat.y() * scalar, quat.z() * scalar };
    }

    template < typename TType >
    Quaternion< TType > operator+( Quaternion< TType > const& lhs, Quaternion< TType > const& rhs )
    {
        return { quat1.w() + quat2.w(), quat1.x() + quat2.x(), quat1.y() + quat2.y(),
                 quat1.z() + quat2.z() };
    }

    template < typename TType >
    Vector< Type, 3 > operator*( Quaternion< TType > const& quat, Vector< Type, 3 > const& vec )
    {
        Quaternion< TType > inv = invert( quat );
        Quaternion< TType > x( 0, vec.x(), vec.y(), vec.z() );
        inv = quat * x * inv;
        return { inv.x(), inv.y(), inv.z() };
    }

    template < typename TType >
    std::ostream& operator<<( std::ostream& stream, Quaternion< TType > const& quat )
    {
        stream << "( " << quat.w() << ", " << quat.x() << ", " << quat.y() << ", " << quat.z()
               << " )";
    }
}
