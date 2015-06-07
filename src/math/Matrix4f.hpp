#pragma once

#include <math/Quaternion.hpp>
#include <math/Vector.hpp>

#include <cstdint>
#include <ostream>

namespace tg
{
    class Matrix4f
    {
        public:
            static Matrix4f fromOrtho(
                float right,
                float top,
                float left,
                float bottom,
                float zNear,
                float zFar );

            static Matrix4f fromPerspective(
                float fovy,
                float aspect,
                float zNear,
                float zFar );

            static Matrix4f fromLookAt(
                float eyeX,
                float eyeY,
                float eyeZ,
                float centerX,
                float centerY,
                float centerZ,
                float upX,
                float upY,
                float upZ );

            static Matrix4f fromRotation(const Quaternion< float >& quat);
            static Matrix4f fromTranslation(const Vector< float, 3 >& vec);
            static Matrix4f fromScale(const Vector< float, 3 >& vec);

            Matrix4f();

            Matrix4f(const Matrix4f& mat);

            Matrix4f(float v0, float v1, float v2, float v3,
                     float v4, float v5, float v6, float v7,
                     float v8, float v9, float v10, float v11,
                     float v12, float v13, float v14, float v15);

            Matrix4f transpose();

            const Matrix4f& operator=(const Matrix4f& mat);

            float operator[](uint8_t i) const { return m_v[i]; }
            float& operator[](uint8_t i) { return m_v[i]; }

            float v(uint8_t i) const { return m_v[i]; }
            void v(uint8_t i, float value) { m_v[i] = value; }

            const float* ptr() const { return m_v; }

            void translate(float x, float y, float z);

            Matrix4f invert3x3();

        private:
            float m_v[16];
    };
}

tg::Matrix4f operator*(const tg::Matrix4f& mat1, const tg::Matrix4f& mat2);
tg::Vector< float, 3 > operator*(const tg::Matrix4f& mat, const tg::Vector< float, 3 >& vec);
tg::Vector< float, 3 > operator*(const tg::Vector< float, 3 >& vec, const tg::Matrix4f& mat);

std::ostream& operator<<(std::ostream& stream, const tg::Matrix4f& matrix);
