#pragma once

#include "math/helper/helper.h"
#include "math/imaginary/complex.h"
#include "math/imaginary/quaternion.h"
#include "math/vector/vector2.h"
#include "math/vector/vector3.h"
#include "math/vector/vector4.h"
#include "math/matrix/matrix2x2.h"
#include "math/matrix/matrix3x3.h"
#include "math/matrix/matrix4x4.h"
#include "math/triangle/vertex.h"
#include "math/triangle/triangle.h"
#include "math/ray/aabb.h"
#include "math/ray/sphere.h"
#include "math/ray/plane.h"
#include "math/ray/ray.h"


namespace kl {
    template<typename T>
    constexpr Complex_T<T> abs(const Complex_T<T>& num)
    {
        return apply<2>((T(*)(T)) abs<T>, num);
    }

    template<typename T>
    constexpr Complex_T<T> min(const Complex_T<T>& num1, const Complex_T<T>& num2)
    {
        return apply<2>((T(*)(T, T)) min<T>, num1, num2);
    }

    template<typename T>
    constexpr Complex_T<T> max(const Complex_T<T>& num1, const Complex_T<T>& num2)
    {
        return apply<2>((T(*)(T, T)) max<T>, num1, num2);
    }

    template<typename T>
    constexpr Complex_T<T> normalize(const Complex_T<T>& num)
    {
        return num * (T(1) / num.length());
    }

    template<typename T>
    constexpr Complex_T<T> inverse(const Complex_T<T>& num)
    {
        const T sqr_sum = num.r * num.r + num.i * num.i;
        if (sqr_sum == T(0)) {
            return {};
        }
        return { num.r / sqr_sum, -num.i / sqr_sum };
    }

    template<typename T>
    constexpr Quaternion_T<T> abs(const Quaternion_T<T>& num)
    {
        return apply<4>((T(*)(T)) abs<T>, num);
    }

    template<typename T>
    constexpr Quaternion_T<T> min(const Quaternion_T<T>& num1, const Quaternion_T<T>& num2)
    {
        return apply<4>((T(*)(T, T)) min<T>, num1, num2);
    }

    template<typename T>
    constexpr Quaternion_T<T> max(const Quaternion_T<T>& num1, const Quaternion_T<T>& num2)
    {
        return apply<4>((T(*)(T, T)) max<T>, num1, num2);
    }

    template<typename T>
    constexpr Quaternion_T<T> normalize(const Quaternion_T<T>& num)
    {
        return num * (T(1) / num.length());
    }

    template<typename T>
    constexpr Quaternion_T<T> inverse(const Quaternion_T<T>& num)
    {
        return { num.w, -num.x, -num.y, -num.z };
    }

    template<typename T>
    constexpr Vector2<T> abs(const Vector2<T>& vec)
    {
        return apply<2>((T(*)(T)) abs<T>, vec);
    }

    template<typename T>
    constexpr Vector2<T> min(const Vector2<T>& vec1, const Vector2<T>& vec2)
    {
        return apply<2>((T(*)(T, T)) min<T>, vec1, vec2);
    }

    template<typename T>
    constexpr Vector2<T> max(const Vector2<T>& vec1, const Vector2<T>& vec2)
    {
        return apply<2>((T(*)(T, T)) max<T>, vec1, vec2);
    }

    template<typename T>
    constexpr Vector2<T> normalize(const Vector2<T>& vec)
    {
        return vec * (T(1) / vec.length());
    }

    template<typename T>
    constexpr T dot(const Vector2<T>& first, const Vector2<T>& second)
    {
        return first.x * second.x + first.y * second.y;
    }

    template<typename T>
    constexpr T angle(Vector2<T> first, Vector2<T> second, bool full)
    {
        first = normalize(first);
        second = normalize(second);

        T result = {};
        if (full) {
            result = std::atan2(first.x * second.y - first.y * second.x, first.x * second.x + first.y * second.y);
            result += pi<T>();
        }
        else {
            result = acos(dot(first, second));
        }
        return result * to_degrees<T>();
    }

    template<typename T>
    constexpr Vector2<T> rotate(const Vector2<T>& vec, T angle)
    {
        const T sin_a = sin_d(angle);
        const T cos_a = cos_d(angle);
        return { cos_a * vec.x - sin_a * vec.y, sin_a * vec.x + cos_a * vec.y };
    }

    template<typename T>
    constexpr Vector2<T> reflect(const Vector2<T>& vec, const Vector2<T>& point)
    {
        const Vector2<T> normalized = normalize(point);
        return vec - (normalized * (vec * normalized) * T(2));
    }

    template<typename T>
    constexpr Vector3<T> abs(const Vector3<T>& vec)
    {
        return apply<3>((T(*)(T)) abs<T>, vec);
    }

    template<typename T>
    constexpr Vector3<T> min(const Vector3<T>& vec1, const Vector3<T>& vec2)
    {
        return apply<3>((T(*)(T, T)) min<T>, vec1, vec2);
    }

    template<typename T>
    constexpr Vector3<T> max(const Vector3<T>& vec1, const Vector3<T>& vec2)
    {
        return apply<3>((T(*)(T, T)) max<T>, vec1, vec2);
    }

    template<typename T>
    constexpr Vector3<T> normalize(const Vector3<T>& vec)
    {
        return vec * (T(1) / vec.length());
    }

    template<typename T>
    constexpr T dot(const Vector3<T>& first, const Vector3<T>& second)
    {
        return first.x * second.x + first.y * second.y + first.z * second.z;
    }

    template<typename T>
    constexpr T angle(const Vector3<T>& first, const Vector3<T>& second)
    {
        return acos_d(dot(normalize(first), normalize(second)));
    }

    template<typename T>
    constexpr Vector3<T> rotate(const Vector3<T>& vec, const Vector3<T>& axis, float angle)
    {
        const Quaternion_T<T> quat{ axis, angle };
        const Quaternion_T<T> inv_quat = inverse(quat);
        return quat * vec * inv_quat;
    }

    template<typename T>
    constexpr Vector3<T> reflect(const Vector3<T>& first, Vector3<T> normal)
    {
        normal = normalize(normal);
        return first - (normal * (first * normal) * T(2));
    }

    template<typename T>
    constexpr Vector3<T> cross(const Vector3<T>& first, const Vector3<T>& second)
    {
        return {
            first.y * second.z - first.z * second.y,
            first.z * second.x - first.x * second.z,
            first.x * second.y - first.y * second.x,
        };
    }

    template<typename T>
    constexpr Vector4<T> abs(const Vector4<T>& vec)
    {
        return apply<4>((T(*)(T)) abs<T>, vec);
    }

    template<typename T>
    constexpr Vector4<T> min(const Vector4<T>& vec1, const Vector4<T>& vec2)
    {
        return apply<4>((T(*)(T, T)) min<T>, vec1, vec2);
    }

    template<typename T>
    constexpr Vector4<T> max(const Vector4<T>& vec1, const Vector4<T>& vec2)
    {
        return apply<4>((T(*)(T, T)) max<T>, vec1, vec2);
    }

    template<typename T>
    constexpr Vector4<T> normalize(const Vector4<T>& vec)
    {
        return vec * (T(1) / vec.length());
    }

    template<typename T>
    constexpr T dot(const Vector4<T>& first, const Vector4<T>& second)
    {
        return first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w;
    }

    template<typename T>
    constexpr T angle(const Vector4<T>& first, const Vector4<T>& second)
    {
        return acos_d(dot(normalize(first), normalize(second)));
    }

    template<typename T>
    constexpr Matrix2x2<T> abs(const Matrix2x2<T>& mat)
    {
        return apply<4>((T(*)(T)) abs<T>, mat);
    }

    template<typename T>
    constexpr Matrix2x2<T> min(const Matrix2x2<T>& mat1, const Matrix2x2<T>& mat2)
    {
        return apply<4>((T(*)(T, T)) min<T>, mat1, mat2);
    }

    template<typename T>
    constexpr Matrix2x2<T> max(const Matrix2x2<T>& mat1, const Matrix2x2<T>& mat2)
    {
        return apply<4>((T(*)(T, T)) max<T>, mat1, mat2);
    }

    template<typename T>
    constexpr Matrix2x2<T> inverse(const Matrix2x2<T>& mat)
    {
        const T determinant = mat.determinant();

        Matrix2x2<T> result;
        result[0] = +mat[3];
        result[1] = -mat[1];
        result[2] = -mat[2];
        result[3] = +mat[0];
        return result * determinant;
    }

    template<typename T>
    constexpr Matrix2x2<T> transpose(const Matrix2x2<T>& mat)
    {
        Matrix2x2<T> result;
        for (int y = 0; y < 2; y++) {
            for (int x = 0; x < 2; x++) {
                result(x, y) = mat(y, x);
            }
        }
        return result;
    }

    template<typename T>
    constexpr Matrix3x3<T> abs(const Matrix3x3<T>& mat)
    {
        return apply<9>((T(*)(T)) abs<T>, mat);
    }

    template<typename T>
    constexpr Matrix3x3<T> min(const Matrix3x3<T>& mat1, const Matrix3x3<T>& mat2)
    {
        return apply<9>((T(*)(T, T)) min<T>, mat1, mat2);
    }

    template<typename T>
    constexpr Matrix3x3<T> max(const Matrix3x3<T>& mat1, const Matrix3x3<T>& mat2)
    {
        return apply<9>((T(*)(T, T)) max<T>, mat1, mat2);
    }

    template<typename T>
    constexpr Matrix3x3<T> inverse(const Matrix3x3<T>& mat)
    {
        const T determinant = mat.determinant();

        Matrix3x3<T> result;
        result(0, 0) = (mat(1, 1) * mat(2, 2) - mat(2, 1) * mat(1, 2)) * determinant;
        result(0, 1) = (mat(0, 2) * mat(2, 1) - mat(0, 1) * mat(2, 2)) * determinant;
        result(0, 2) = (mat(0, 1) * mat(1, 2) - mat(0, 2) * mat(1, 1)) * determinant;
        result(1, 0) = (mat(1, 2) * mat(2, 0) - mat(1, 0) * mat(2, 2)) * determinant;
        result(1, 1) = (mat(0, 0) * mat(2, 2) - mat(0, 2) * mat(2, 0)) * determinant;
        result(1, 2) = (mat(1, 0) * mat(0, 2) - mat(0, 0) * mat(1, 2)) * determinant;
        result(2, 0) = (mat(1, 0) * mat(2, 1) - mat(2, 0) * mat(1, 1)) * determinant;
        result(2, 1) = (mat(2, 0) * mat(0, 1) - mat(0, 0) * mat(2, 1)) * determinant;
        result(2, 2) = (mat(0, 0) * mat(1, 1) - mat(1, 0) * mat(0, 1)) * determinant;
        return result;
    }

    template<typename T>
    constexpr Matrix3x3<T> transpose(const Matrix3x3<T>& mat)
    {
        Matrix3x3<T> result;
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                result(x, y) = mat(y, x);
            }
        }
        return result;
    }

    template<typename T>
    constexpr Matrix4x4<T> abs(const Matrix4x4<T>& mat)
    {
        return apply<16>((T(*)(T)) abs<T>, mat);
    }

    template<typename T>
    constexpr Matrix4x4<T> min(const Matrix4x4<T>& mat1, const Matrix4x4<T>& mat2)
    {
        return apply<16>((T(*)(T, T)) min<T>, mat1, mat2);
    }

    template<typename T>
    constexpr Matrix4x4<T> max(const Matrix4x4<T>& mat1, const Matrix4x4<T>& mat2)
    {
        return apply<16>((T(*)(T, T)) max<T>, mat1, mat2);
    }

    template<typename T>
    constexpr Matrix4x4<T> inverse(const Matrix4x4<T>& mat)
    {
        const T determinant = mat.determinant();

        const T a_2323 = mat(2, 2) * mat(3, 3) - mat(3, 2) * mat(2, 3);
        const T a_1323 = mat(1, 2) * mat(3, 3) - mat(3, 2) * mat(1, 3);
        const T a_1223 = mat(1, 2) * mat(2, 3) - mat(2, 2) * mat(1, 3);
        const T a_0323 = mat(0, 2) * mat(3, 3) - mat(3, 2) * mat(0, 3);
        const T a_0223 = mat(0, 2) * mat(2, 3) - mat(2, 2) * mat(0, 3);
        const T a_0123 = mat(0, 2) * mat(1, 3) - mat(1, 2) * mat(0, 3);
        const T a_2313 = mat(2, 1) * mat(3, 3) - mat(3, 1) * mat(2, 3);
        const T a_1313 = mat(1, 1) * mat(3, 3) - mat(3, 1) * mat(1, 3);
        const T a_1213 = mat(1, 1) * mat(2, 3) - mat(2, 1) * mat(1, 3);
        const T a_2312 = mat(2, 1) * mat(3, 2) - mat(3, 1) * mat(2, 2);
        const T a_1312 = mat(1, 1) * mat(3, 2) - mat(3, 1) * mat(1, 2);
        const T a_1212 = mat(1, 1) * mat(2, 2) - mat(2, 1) * mat(1, 2);
        const T a_0313 = mat(0, 1) * mat(3, 3) - mat(3, 1) * mat(0, 3);
        const T a_0213 = mat(0, 1) * mat(2, 3) - mat(2, 1) * mat(0, 3);
        const T a_0312 = mat(0, 1) * mat(3, 2) - mat(3, 1) * mat(0, 2);
        const T a_0212 = mat(0, 1) * mat(2, 2) - mat(2, 1) * mat(0, 2);
        const T a_0113 = mat(0, 1) * mat(1, 3) - mat(1, 1) * mat(0, 3);
        const T a_0112 = mat(0, 1) * mat(1, 2) - mat(1, 1) * mat(0, 2);

        Matrix4x4<T> result;
        result(0, 0) = +(mat(1, 1) * a_2323 - mat(2, 1) * a_1323 + mat(3, 1) * a_1223) * determinant;
        result(1, 0) = -(mat(1, 0) * a_2323 - mat(2, 0) * a_1323 + mat(3, 0) * a_1223) * determinant;
        result(2, 0) = +(mat(1, 0) * a_2313 - mat(2, 0) * a_1313 + mat(3, 0) * a_1213) * determinant;
        result(3, 0) = -(mat(1, 0) * a_2312 - mat(2, 0) * a_1312 + mat(3, 0) * a_1212) * determinant;
        result(0, 1) = -(mat(0, 1) * a_2323 - mat(2, 1) * a_0323 + mat(3, 1) * a_0223) * determinant;
        result(1, 1) = +(mat(0, 0) * a_2323 - mat(2, 0) * a_0323 + mat(3, 0) * a_0223) * determinant;
        result(2, 1) = -(mat(0, 0) * a_2313 - mat(2, 0) * a_0313 + mat(3, 0) * a_0213) * determinant;
        result(3, 1) = +(mat(0, 0) * a_2312 - mat(2, 0) * a_0312 + mat(3, 0) * a_0212) * determinant;
        result(0, 2) = +(mat(0, 1) * a_1323 - mat(1, 1) * a_0323 + mat(3, 1) * a_0123) * determinant;
        result(1, 2) = -(mat(0, 0) * a_1323 - mat(1, 0) * a_0323 + mat(3, 0) * a_0123) * determinant;
        result(2, 2) = +(mat(0, 0) * a_1313 - mat(1, 0) * a_0313 + mat(3, 0) * a_0113) * determinant;
        result(3, 2) = -(mat(0, 0) * a_1312 - mat(1, 0) * a_0312 + mat(3, 0) * a_0112) * determinant;
        result(0, 3) = -(mat(0, 1) * a_1223 - mat(1, 1) * a_0223 + mat(2, 1) * a_0123) * determinant;
        result(1, 3) = +(mat(0, 0) * a_1223 - mat(1, 0) * a_0223 + mat(2, 0) * a_0123) * determinant;
        result(2, 3) = -(mat(0, 0) * a_1213 - mat(1, 0) * a_0213 + mat(2, 0) * a_0113) * determinant;
        result(3, 3) = +(mat(0, 0) * a_1212 - mat(1, 0) * a_0212 + mat(2, 0) * a_0112) * determinant;
        return result;
    }

    template<typename T>
    constexpr Matrix4x4<T> transpose(const Matrix4x4<T>& mat)
    {
        Matrix4x4<T> result;
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                result(x, y) = mat(y, x);
            }
        }
        return result;
    }

    template<typename T>
    constexpr Vector2<T> calc_ndc(const Vector2<T>& position, const Vector2<T>& frame_size)
    {
        const Vector2<T> result = {
            position.x / frame_size.x,
            (frame_size.y - position.y) / frame_size.y,
        };
        return result * T(2) - Vector2<T>(T(1));
    }

    template<typename T>
    constexpr Vector2<T> calc_ndc_ar(const Vector2<T>& position, const Vector2<T>& size)
    {
        Vector2<T> result = calc_ndc(position, size);
        result.x *= size.x / size.y;
        return result;
    }

    template<typename T>
    constexpr T line_x(const Vector2<T>& a, const Vector2<T>& b, T y)
    {
        return T(((y - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x);
    }

    template<typename T>
    constexpr T line_y(const Vector2<T>& a, const Vector2<T>& b, T x)
    {
        return T(((b.y - a.y) * (x - a.x)) / (b.x - a.x) + a.y);
    }

    template<typename T>
    constexpr Quaternion_T<T> to_quaternion(const Vector3<T>& euler)
    {
        const T cr = cos_d(euler.x * T(0.5));
        const T sr = sin_d(euler.x * T(0.5));
        const T cp = cos_d(euler.y * T(0.5));
        const T sp = sin_d(euler.y * T(0.5));
        const T cy = cos_d(euler.z * T(0.5));
        const T sy = sin_d(euler.z * T(0.5));
        return {
            cr * cp * cy + sr * sp * sy,
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy,
        };
    }

    template<typename T>
    constexpr Vector3<T> to_euler(const Quaternion_T<T>& quat)
    {
        const T       sin_p = T(+2) * (quat.w * quat.y - quat.z * quat.x) + T(0);
        const T sin_r_cos_p = T(+2) * (quat.w * quat.x + quat.y * quat.z) + T(0);
        const T cos_r_cos_p = T(-2) * (quat.x * quat.x + quat.y * quat.y) + T(1);
        const T sin_y_cos_p = T(+2) * (quat.w * quat.z + quat.x * quat.y) + T(0);
        const T cos_y_cos_p = T(-2) * (quat.y * quat.y + quat.z * quat.z) + T(1);

        const Vector3<T> result = {
            std::atan2(sin_r_cos_p, cos_r_cos_p),
            abs(sin_p) >= T(1) ? (T) std::copysign(pi<T>() * T(0.5), sin_p) : (T) asin(sin_p),
            std::atan2(sin_y_cos_p, cos_y_cos_p),
        };
        return result * to_degrees<T>();
    }

    template<typename T>
    constexpr Quaternion_T<T> to_quaternion(const Vector3<T>& original, const Vector3<T>& target)
    {
        const Vector3<T> axis = normalize(cross(original, target));
        const T angl = angle(original, target);
        return Quaternion_T{ axis, angl };
    }

    template<typename T>
    constexpr Vector3<T> to_euler(const Vector3<T>& original, const Vector3<T>& target)
    {
        return to_euler(to_quaternion(original, target));
    }
}
