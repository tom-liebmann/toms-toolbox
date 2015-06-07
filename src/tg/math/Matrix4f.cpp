#include "Matrix4f.hpp"

#include <cmath>

namespace tg
{
    Matrix4f Matrix4f::fromOrtho(
        float right, float top, float left, float bottom, float zNear, float zFar )
    {
        return Matrix4f(2.0f / (right - left), 0.0f                 , 0.0f                 , (right + left) / (left - right),
                        0.0f                 , 2.0f / (top - bottom), 0.0f                 , (top + bottom) / (bottom - top),
                        0.0f                 , 0.0f                 , 2.0f / (zFar - zNear), (zFar + zNear) / (zNear - zFar),
                        0.0f                 , 0.0f                 , 0.0f                 , 1.0f);
    }

    Matrix4f Matrix4f::fromPerspective(float fovy, float aspect, float zNear, float zFar)
    {
        fovy = 1.0f / std::tan(fovy / 2.0f / 57.2958f);

        return Matrix4f(fovy / aspect, 0.0f, 0.0f                           , 0.0f,
                        0.0f         , fovy, 0.0f                           , 0.0f,
                        0.0f         , 0.0f, (zFar + zNear) / (zNear - zFar), 2.0f * zFar * zNear / (zNear - zFar),
                        0.0f         , 0.0f, -1.0f                          , 0.0f);
    }

    Matrix4f Matrix4f::fromLookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
    {
        Vector< float, 3 > F = { centerX - eyeX, centerY - eyeY, centerZ - eyeZ };
        F /= norm( F );
        Vector< float, 3 > u = { upX, upY, upZ };

        Vector< float, 3 > s = cross( F, u );
        s /= norm( s );
        u = cross( s, F );

        return Matrix4f(s[0] , s[1] , s[2] , 0.0f,
                        u[0] , u[1] , u[2] , 0.0f,
                        -F[0], -F[1], -F[2], 0.0f,
                        0.0f , 0.0f , 0.0f , 1.0f)
             * Matrix4f(1.0f, 0.0f, 0.0f, -eyeX,
                        0.0f, 1.0f, 0.0f, -eyeY,
                        0.0f, 0.0f, 1.0f, -eyeZ,
                        0.0f, 0.0f, 0.0f, 1.0f);
    }

    Matrix4f Matrix4f::fromRotation( const Quaternion< float >& quat )
    {
        float x2 = quat.x() * quat.x();
        float y2 = quat.y() * quat.y();
        float z2 = quat.z() * quat.z();
        float xy = quat.x() * quat.y();
        float xz = quat.x() * quat.z();
        float yz = quat.y() * quat.z(); 
        float wx = quat.w() * quat.x();
        float wy = quat.w() * quat.y();
        float wz = quat.w() * quat.z();

        return Matrix4f(
            1.0f - 2.0f * ( y2 + z2 ),        2.0f * ( xy - wz ),        2.0f * ( xz + wy ), 0.0f,
                   2.0f * ( xy + wz ), 1.0f - 2.0f * ( x2 + z2 ),        2.0f * ( yz - wx ), 0.0f,
                   2.0f * ( xz - wy ),        2.0f * ( yz + wx ), 1.0f - 2.0f * ( x2 + y2 ), 0.0f,
            0.0f                     , 0.0f                     , 0.0f                     , 1.0f );
    }

    Matrix4f Matrix4f::fromTranslation(const Vector< float, 3 >& vec)
    {
        return Matrix4f(1.0f, 0.0f, 0.0f, vec[0],
                        0.0f, 1.0f, 0.0f, vec[1],
                        0.0f, 0.0f, 1.0f, vec[2],
                        0.0f, 0.0f, 0.0f, 1.0f);
    }

    Matrix4f Matrix4f::fromScale(const Vector< float, 3 >& vec)
    {
        return Matrix4f(vec[0], 0.0f  , 0.0f  , 0.0f,
                        0.0f  , vec[1], 0.0f  , 0.0f,
                        0.0f  , 0.0f  , vec[2], 0.0f,
                        0.0f  , 0.0f  , 0.0f  , 1.0f);
    }

    Matrix4f::Matrix4f() : 
        m_v { 1, 0, 0, 0,
              0, 1, 0, 0,
              0, 0, 1, 0,
              0, 0, 0, 1 }
    { }

    Matrix4f::Matrix4f( const Matrix4f& mat ) :
        m_v { mat[0],  mat[1],  mat[2],  mat[3],
              mat[4],  mat[5],  mat[6],  mat[7],
              mat[8],  mat[9],  mat[10], mat[11],
              mat[12], mat[13], mat[14], mat[15] }
    { }

    Matrix4f::Matrix4f( float v0, float v1, float v2, float v3,
                        float v4, float v5, float v6, float v7,
                        float v8, float v9, float v10, float v11,
                        float v12, float v13, float v14, float v15 ) :
        m_v { v0,  v1,  v2,  v3,
              v4,  v5,  v6,  v7,
              v8,  v9,  v10, v11,
              v12, v13, v14, v15 }
    { }

    Matrix4f Matrix4f::transpose()
    {
        return Matrix4f(m_v[0], m_v[4], m_v[8], m_v[12],
                        m_v[1], m_v[5], m_v[9], m_v[13],
                        m_v[2], m_v[6], m_v[10], m_v[14],
                        m_v[3], m_v[7], m_v[11], m_v[15]);
    }

    const Matrix4f& Matrix4f::operator=(const Matrix4f& mat)
    {
        for(int i = 0; i < 16; ++i)
            m_v[i] = mat[i];
        return *this;
    }

    void Matrix4f::translate(float x, float y, float z)
    {
        m_v[3]  += m_v[0]  * x + m_v[1]  * y + m_v[2] * z;
        m_v[7]  += m_v[4]  * x + m_v[5]  * y + m_v[6] * z;
        m_v[11] += m_v[8]  * x + m_v[9]  * y + m_v[10] * z;
        m_v[15] += m_v[12] * x + m_v[13] * y + m_v[14] * z;
    }

    Matrix4f Matrix4f::invert3x3()
    {
        float det = m_v[0] * m_v[5] * m_v[10]
                  + m_v[1] * m_v[6] * m_v[8]
                  + m_v[4] * m_v[9] * m_v[2]
                  - m_v[0] * m_v[6] * m_v[9]
                  - m_v[2] * m_v[5] * m_v[8]
                  - m_v[1] * m_v[4] * m_v[10];

        return Matrix4f((m_v[5] * m_v[10] - m_v[6] * m_v[9]) / det,
                        (m_v[6] * m_v[8] - m_v[4] * m_v[10]) / det,
                        (m_v[4] * m_v[9] - m_v[5] * m_v[8]) / det,
                        0.0f,
                        (m_v[2] * m_v[9] - m_v[1] * m_v[10]) / det,
                        (m_v[0] * m_v[10] - m_v[2] * m_v[8]) / det,
                        (m_v[1] * m_v[8] - m_v[0] * m_v[9]) / det,
                        0.0f,
                        (m_v[1] * m_v[6] - m_v[2] * m_v[5]) / det,
                        (m_v[2] * m_v[4] - m_v[0] * m_v[6]) / det,
                        (m_v[0] * m_v[5] - m_v[1] * m_v[4]) / det,
                        0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f);
    }

    Matrix4f operator*(const Matrix4f& mat1, const Matrix4f& mat2)
    {
        float v[16];
        for(int x = 0; x < 4; x++)
        for(int y = 0; y < 4; y++)
        {
            v[x + y * 4] = 0;
            for(int i = 0; i < 4; i++)
                v[x + y * 4] += mat1[i + y * 4] * mat2[x + i * 4];
        }

        return Matrix4f(v[0], v[1], v[2], v[3],
                        v[4], v[5], v[6], v[7],
                        v[8], v[9], v[10], v[11],
                        v[12], v[13], v[14], v[15]);
    }

    Vector< float, 3 > operator*(const Matrix4f& mat, const Vector< float, 3 >& vec)
    {
        float v[4];
        for(int y = 0; y < 4; y++)
        {
            v[y] = 0;
            for(int i = 0; i < 4; i++)
                v[y] += mat[i + y * 4] * (i < 3 ? vec[i] : 1.0f);
        }
        
        if(v[3] < -1e-7 || v[3] > 1e-7)
        {
            v[0] /= v[3];
            v[1] /= v[3];
            v[2] /= v[3];
        }

        return Vector< float, 3 >( { v[0], v[1], v[2] } );
    }
}

std::ostream& operator<<(std::ostream& stream, const tg::Matrix4f& matrix)
{
    return stream
    << "/"  << matrix[0]  << ", " << matrix[1]  << ", " << matrix[2]  << ", " << matrix[3]  << "\\" << std::endl
    << "|"  << matrix[4]  << ", " << matrix[5]  << ", " << matrix[6]  << ", " << matrix[7]  << "|"  << std::endl
    << "|"  << matrix[8]  << ", " << matrix[9]  << ", " << matrix[10] << ", " << matrix[11] << "|"  << std::endl
    << "\\" << matrix[12] << ", " << matrix[13] << ", " << matrix[14] << ", " << matrix[15] << "/";
}

