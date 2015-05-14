#pragma once

#include <cmath>
#include <cstdlib>
#include <ostream>
#include <array>

//namespace tg
//{
//    template< typename T, size_t D >
//    class Vector
//    {
//    public:
//        // Construction
//        template< typename... K >
//        Vector( K&&... values );
//
//        Vector( std::initializer_list< T > values );
//
//        // Access
//        T& operator[]( size_t index );
//        const T& operator[]( size_t index ) const;
//
//        T& x();
//        T& y();
//        T& z();
//
//        const T& x() const;
//        const T& y() const;
//        const T& z() const;
//
//    private:
//        std::array< T, D > m_values;
//    };
//
//
//
//    // Norm of a vector.
//    template< typename T, size_t D >
//    T norm( const Vector< T, D >& vector );
//
//    // Squared norm of a vector.
//    template< typename T, size_t D >
//    T norm2( const Vector< T, D >& vector );
//
//    // Sum of two vectors.
//    template< typename T1, typename T2, size_t D >
//    Vector< decltype( T1() + T2() ), D > operator+( const Vector< T1, D >& lhs, const Vector< T2, D >& rhs );
//
//    // Subtraction of two vectors.
//    template< typename T1, typename T2, size_t D >
//    Vector< decltype( T1() + T2() ), D > operator-( const Vector< T1, D >& lhs, const Vector< T2, D >& rhs );
//
//    // Dot product of two vectors.
//    template< typename T1, typename T2, size_t D >
//    decltype( T1() * T2() ) operator*( const Vector< T1, D >& lhs, const Vector< T2, D >& rhs );
//
//    // Product of vector with scalar.
//    template< typename T, size_t D, typename S >
//    Vector< decltype( T() * S() ), D > operator*( const S& lhs, const Vector< T, D >& rhs );
//
//    // Product of vector with scalar.
//    template< typename T, size_t D, typename S >
//    Vector< decltype( T() * S() ), D > operator*( const Vector< T, D >& lhs, const S& rhs );
//}



namespace tgMath
{
    template<typename Scalar, size_t Size>
    class Vector
    {
        public:
            static Vector<Scalar, Size> zeroVector;

            Vector()
            {
                for(size_t i = 0; i < Size; ++i)
                    m_values[i] = 0;
            }

            Vector(const Vector<Scalar, Size>& vec)
            {
                for(size_t i = 0; i < Size; ++i)
                    m_values[i] = vec[i];
            }

            Vector(float x, float y)
            {
//                    Assert(Size == 2, "Wrong dimension count");
                m_values[0] = x;
                m_values[1] = y;
            }

            Vector(float x, float y, float z)
            {
//                    Assert(Size == 3, "Wrong dimension count");
                m_values[0] = x;
                m_values[1] = y;
                m_values[2] = z;
            }

            Vector(float x, float y, float z, float w)
            {
//                    Assert(Size == 4, "Wrong dimension count");
                m_values[0] = x;
                m_values[1] = y;
                m_values[2] = z;
                m_values[3] = w;
            }

            inline const Scalar& operator[](const size_t index) const
            {
//                    Assert(index < Size, "Invalid vector-index");
                return m_values[index];
            }

            inline Scalar& operator[](const size_t index)
            {
//                    Assert(index < Size, "Invalid vector-index");
                return m_values[index];
            }

            inline const Scalar& x() const { return (*this)[0]; }
            inline const Scalar& y() const { return (*this)[1]; }
            inline const Scalar& z() const { return (*this)[2]; }
            inline const Scalar& w() const { return (*this)[3]; }

            inline const Scalar& r() const { return (*this)[0]; }
            inline const Scalar& g() const { return (*this)[1]; }
            inline const Scalar& b() const { return (*this)[2]; }
            inline const Scalar& a() const { return (*this)[3]; }

            inline Scalar& x() { return (*this)[0]; }
            inline Scalar& y() { return (*this)[1]; }
            inline Scalar& z() { return (*this)[2]; }
            inline Scalar& w() { return (*this)[3]; }

            inline Scalar& r() { return (*this)[0]; }
            inline Scalar& g() { return (*this)[1]; }
            inline Scalar& b() { return (*this)[2]; }
            inline Scalar& a() { return (*this)[3]; }

            inline double length() const
            {
                return std::sqrt(lengthSq());
            }

            inline double lengthSq() const;

            inline const Vector<Scalar, Size>& operator=(const Vector<Scalar, Size>& vector)
            {
                for(size_t i = 0; i < Size; ++i)
                    m_values[i] = vector[i];
                return *this;
            }

            inline const Vector<Scalar, Size>& operator+=(const Vector<Scalar, Size>& vector)
            {
                for(size_t i = 0; i < Size; ++i)
                    m_values[i] += vector[i];
                return *this;
            }

            inline const Vector<Scalar, Size>& operator-=(const Vector<Scalar, Size>& vector)
            {
                for(size_t i = 0; i < Size; ++i)
                    m_values[i] -= vector[i];
                return *this;
            }

            Vector<Scalar, Size> normalize() const
            {
                double len = length();
                if(std::abs(len) > 1e-7)
                {
                    Vector<Scalar, Size> result;
                    for(size_t i = 0; i < Size; ++i)
                        result[i] = m_values[i] / len;
                    return result;
                }
                else
                    return *this;
            }

        private:
            Scalar m_values[Size];
    };
}

template<typename Scalar, size_t Size>
tgMath::Vector<Scalar, Size> tgMath::Vector<Scalar, Size>::zeroVector;

template<typename Scalar, size_t Size>
tgMath::Vector<Scalar, Size> operator+(
    const tgMath::Vector<Scalar, Size>& vec1,
    const tgMath::Vector<Scalar, Size>& vec2)
{
    tgMath::Vector<Scalar, Size> result;
    for(size_t i = 0; i < Size; ++i) 
        result[i] = vec1[i] + vec2[i];
    return result;
}

template<typename Scalar, size_t Size>
tgMath::Vector<Scalar, Size> operator-(
    const tgMath::Vector<Scalar, Size>& vec1,
    const tgMath::Vector<Scalar, Size>& vec2)
{
    tgMath::Vector<Scalar, Size> result;
    for(size_t i = 0; i < Size; ++i) 
        result[i] = vec1[i] - vec2[i];
    return result;
}

template<typename Scalar, size_t Size>
float operator*(
    const tgMath::Vector<Scalar, Size>& vec1,
    const tgMath::Vector<Scalar, Size>& vec2)
{
    float result = 0.0f;
    for(size_t i = 0; i < Size; ++i)
        result += vec1[i] * vec2[i];
    return result;
}

template<typename Scalar>
tgMath::Vector<Scalar, 3> operator%(
    const tgMath::Vector<Scalar, 3>& vec1,
    const tgMath::Vector<Scalar, 3>& vec2)
{
    return tgMath::Vector<Scalar, 3>(
        vec1[1] * vec2[2] - vec1[2] * vec2[1],
        vec1[2] * vec2[0] - vec1[0] * vec2[2],
        vec1[0] * vec2[1] - vec1[1] * vec2[0]);
}

template<typename Scalar, size_t Size>
tgMath::Vector<Scalar, Size> operator*(
    const tgMath::Vector<Scalar, Size>& vec, Scalar scalar)
{
    tgMath::Vector<Scalar, Size> result;
    for(size_t i = 0; i < Size; ++i)
        result[i] = vec[i] * scalar;
    return result;
}

template<typename Scalar, size_t Size>
tgMath::Vector<Scalar, Size> operator*(
    Scalar scalar, const tgMath::Vector<Scalar, Size>& vec)
{
    tgMath::Vector<Scalar, Size> result;
    for(size_t i = 0; i < Size; ++i)
        result[i] = vec[i] * scalar;
    return result;
}

template<typename Scalar, size_t Size>
tgMath::Vector<Scalar, Size> operator/(
    const tgMath::Vector<Scalar, Size>& vec, Scalar scalar)
{
    tgMath::Vector<Scalar, Size> result;
    for(size_t i = 0; i < Size; ++i)
        result[i] = vec[i] / scalar;
    return result;
}

template<typename Scalar, size_t Size>
std::ostream& operator<<(
    std::ostream& stream, const tgMath::Vector<Scalar, Size>& vec)
{
    stream << "(";
    for(size_t i = 0; i < Size; ++i)
    {
        if(i != 0)
            stream << ", ";
        stream << vec[i];
    }
    stream << ")";
}


namespace tgMath
{
    template<typename Scalar, size_t Size>
    inline double Vector< Scalar, Size >::lengthSq() const
    {
        return (*this) * (*this);
    }
}
