#pragma once

#include "math/matrix/matrix.h"


namespace kl {
    template <typename T>
    struct matrix4x4 : matrix<T, 4, 4>
    {
        matrix4x4()
        {}

        matrix4x4(const vector4<T>& r0, const vector4<T>& r1, const vector4<T>& r2, const vector4<T>& r3)
        {
            (*this)[0] = r0.x;
            (*this)[1] = r0.y;
            (*this)[2] = r0.z;
            (*this)[3] = r0.w;
            (*this)[4] = r1.x;
            (*this)[5] = r1.y;
            (*this)[6] = r1.z;
            (*this)[7] = r1.w;
            (*this)[8] = r2.x;
            (*this)[9] = r2.y;
            (*this)[10] = r2.z;
            (*this)[11] = r2.w;
            (*this)[12] = r3.x;
            (*this)[13] = r3.y;
            (*this)[14] = r3.z;
            (*this)[15] = r3.w;
        }

        matrix4x4(const matrix<T, 4, 4>& mat)
            : matrix<T, 4, 4>(mat)
        {}

        static matrix4x4<T> translation(const vector3<T>& translation)
        {
            matrix4x4<T> result;
            result[3] = translation.x;
            result[7] = translation.y;
            result[11] = translation.z;
            return result;
        }

        static matrix4x4<T> rotation(const vector3<T>& rotation)
        {
            const T x_rad = math::to_radians(rotation.x);
            const T x_sin = std::sin(x_rad);
            const T x_cos = std::cos(x_rad);

            matrix4x4<T> x_rot;
            x_rot[5] = x_cos;
            x_rot[6] = -x_sin;
            x_rot[9] = x_sin;
            x_rot[10] = x_cos;

            const T y_rad = math::to_radians(rotation.y);
            const T y_sin = std::sin(y_rad);
            const T y_cos = std::cos(y_rad);

            matrix4x4<T> y_rot;
            y_rot[0] = y_cos;
            y_rot[2] = y_sin;
            y_rot[8] = -y_sin;
            y_rot[10] = y_cos;

            const T z_rad = math::to_radians(rotation.z);
            const T z_sin = std::sin(z_rad);
            const T z_cos = std::cos(z_rad);

            matrix4x4<T> z_rot;
            z_rot[0] = z_cos;
            z_rot[1] = -z_sin;
            z_rot[4] = z_sin;
            z_rot[5] = z_cos;

            return z_rot * y_rot * x_rot;
        }

        static matrix4x4<T> scaling(const vector3<T>& scale)
        {
            matrix4x4<T> result;
            result[0] = scale.x;
            result[5] = scale.y;
            result[10] = scale.z;
            return result;
        }

        static matrix4x4<T> perspective(T field_of_view, T aspect_ratio, T near_plane, T far_plane)
        {
            const T tan_half = T(1.0f / std::tan(math::to_radians(field_of_view) * 0.5f));

            matrix4x4<T> result;
            result[0] = tan_half / aspect_ratio;
            result[5] = tan_half;
            result[10] = (-far_plane - near_plane) / (near_plane - far_plane);
            result[11] = T(2.0f * near_plane * far_plane) / (near_plane - far_plane);
            result[14] = T(1.0f);
            result[15] = T(0.0f);
            return result;
        }

        static matrix4x4<T> orthographic(T left, T right, T bottom, T top, T near_plane, T far_plane)
        {
            matrix4x4<T> result;
            result[0] = T(2.0f) / (right - left);
            result[5] = T(2.0f) / (top - bottom);
            result[10] = T(2.0f) / (far_plane - near_plane);
            result[3] = -(right + left) / (right - left);
            result[7] = -(top + bottom) / (top - bottom);
            result[11] = -(far_plane + near_plane) / (far_plane - near_plane);
            return result;
        }

        static matrix4x4<T> look_at(const vector3<T>& position, const vector3<T>& target, const vector3<T>& up)
        {
            const vector3<T> f = (target - position).normalize();
            const vector3<T> s = up.cross(f).normalize();
            const vector3<T> u = f.cross(s);

            matrix4x4<T> result;
            result[0] = s.x;
            result[1] = s.y;
            result[2] = s.z;
            result[3] = -s.dot(position);
            result[4] = u.x;
            result[5] = u.y;
            result[6] = u.z;
            result[7] = -u.dot(position);
            result[8] = f.x;
            result[9] = f.y;
            result[10] = f.z;
            result[11] = -f.dot(position);
            return result;
        }
    };
}

namespace kl {
    using float4x4 = matrix4x4<float>;
    using double4x4 = matrix4x4<double>;

    using mat4 = float4x4;
    using dmat4 = double4x4;
}
