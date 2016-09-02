#pragma once

#include <tg/math/Vector.hpp>

#include <ostream>
#include <cmath>

// declarations
//=============================================================================

namespace tg
{
    template< class Type >
    class Quaternion
    {
        public:
            Quaternion();
            Quaternion( Type w, Type x, Type y, Type z);
            Quaternion( double rot, const Vector< Type, 3 >& axis );
            Quaternion( const Quaternion< Type >& quat );
            Quaternion( const Vector< Type, 3 >& from, const Vector< Type, 3 >& to );

            Type w() const { return m_w; }
            Type x() const { return m_x; }
            Type y() const { return m_y; }
            Type z() const { return m_z; }

            Type& w() { return m_w; }
            Type& x() { return m_x; }
            Type& y() { return m_y; }
            Type& z() { return m_z; }

            Type lengthSq() const;
            Type azimuth() const;
            Type polar() const;

            Quaternion< Type > invert() const;
            Quaternion< Type > normalize() const;

            const Quaternion< Type >& operator=( const Quaternion< Type >& quat );

        protected:
            Type m_w;
            Type m_x;
            Type m_y;
            Type m_z;
    };
}



// definitions
//=============================================================================

namespace tg
{
    template< class Type >
    inline Quaternion< Type >::Quaternion()
        : m_w( 1 )
        , m_x( 0 )
        , m_y( 0 )
        , m_z( 0 )
    {
    }

    template< class Type >
    inline Quaternion< Type >::Quaternion( Type w, Type x, Type y, Type z )
        : m_w( w )
        , m_x( x )
        , m_y( y )
        , m_z( z )
    {
    }

    template< class Type >
    Quaternion< Type >::Quaternion( double rot, const Vector< Type, 3 >& axis )
    {
        rot /= 114.5915590262;
        double sin = std::sin( rot );
        Vector< Type, 3 > normAxis = axis / norm( axis );
        m_w = std::cos( rot );
        m_x = normAxis.x() * sin;
        m_y = normAxis.y() * sin;
        m_z = normAxis.z() * sin;
    }

    template< class Type >
    inline Quaternion< Type >::Quaternion( const Quaternion< Type >& quat )
        : m_w( quat.m_w )
        , m_x( quat.m_x )
        , m_y( quat.m_y )
        , m_z( quat.m_z )
    {
    }

    template< class Type >
    Quaternion< Type >::Quaternion( const Vector< Type, 3 >& from, const Vector< Type, 3 >& to )
    {
        auto nf = from / norm( from );
        auto nt = to / norm( to );
        Vector< Type, 3 > halfVec = nf + nt;
        auto len = norm( halfVec );
        halfVec = halfVec / len;
        m_w = dot( halfVec, nt );
        halfVec = cross( halfVec, nt );
        m_x = halfVec.x();
        m_y = halfVec.y();
        m_z = halfVec.z();
    }

    template< class Type >
    inline Type Quaternion< Type >::lengthSq() const
    {
        return m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z; 
    }

    template< class Type >
    inline Type Quaternion< Type >::azimuth() const
    {
        return atan2( m_y, m_x ) * 114.5915590262;
    }

    template< class Type >
    inline Type Quaternion< Type >::polar() const
    {
        return atan2( m_y, m_z ) * 114.5915590262;
    }

    template< class Type >
    inline Quaternion< Type > Quaternion< Type >::invert() const
    {
        return Quaternion< Type >( m_w, -m_x, -m_y, -m_z );
    }

    template< class Type >
    inline Quaternion< Type > Quaternion< Type >::normalize() const
    {
        Type len = sqrt( lengthSq() );
        return Quaternion< Type >( m_w / len, m_x / len, m_y / len, m_z / len );
    }

    template< class Type >
    inline const Quaternion< Type >& Quaternion< Type >::operator=( const Quaternion< Type >& quat )
    {
        m_w = quat.w();
        m_x = quat.x();
        m_y = quat.y();
        m_z = quat.z();
        return *this;
    }
}

template< class Type >
tg::Quaternion< Type > operator*(
    const tg::Quaternion< Type >& quat1,
    const tg::Quaternion< Type >& quat2)
{
    return tg::Quaternion< Type >(
        quat1.w() * quat2.w() - quat1.x() * quat2.x() - quat1.y() * quat2.y() - quat1.z() * quat2.z(),
        quat1.w() * quat2.x() + quat1.x() * quat2.w() + quat1.y() * quat2.z() - quat1.z() * quat2.y(),
        quat1.w() * quat2.y() + quat1.y() * quat2.w() + quat1.z() * quat2.x() - quat1.x() * quat2.z(),
        quat1.w() * quat2.z() + quat1.z() * quat2.w() + quat1.x() * quat2.y() - quat1.y() * quat2.x() );
}

template< class Type >
tg::Quaternion< Type > operator*(
    const tg::Quaternion< Type >& quat,
    Type scalar )
{
    return tg::Quaternion< Type >(
        quat.w() * scalar,
        quat.x() * scalar,
        quat.y() * scalar,
        quat.z() * scalar );
}

template< class Type >
tg::Quaternion< Type > operator+(
    const tg::Quaternion< Type >& quat1,
    const tg::Quaternion< Type >& quat2)
{
    return tg::Quaternion< Type >(
        quat1.w() + quat2.w(),
        quat1.x() + quat2.x(),
        quat1.y() + quat2.y(),
        quat1.z() + quat2.z() );
}

template< class Type >
tg::Vector< Type, 3 > operator*(
    const tg::Quaternion< Type >& quat,
    const tg::Vector< Type, 3 >& vec )
{
    tg::Quaternion< Type > inv = quat.invert();
    tg::Quaternion< Type > x( 0, vec.x(), vec.y(), vec.z() );
    inv = quat * x * inv;
    return tg::Vector< Type, 3 >( { inv.x(), inv.y(), inv.z() } );
}


template< class Type >
std::ostream& operator <<(
    std::ostream& stream,
    const tg::Quaternion< Type >& quat )
{
    stream << "( " << quat.w()
           << ", " << quat.x()
           << ", " << quat.y()
           << ", " << quat.z() << " )";
}
