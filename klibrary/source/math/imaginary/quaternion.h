#pragma once

#include "math/imaginary/complex.h"


namespace kl {
    template<typename T = float>
    struct Quaternion
    {
        T w = {};
        T x = {};
        T y = {};
        T z = {};

        constexpr Quaternion()
        {}

        constexpr Quaternion(T x, T y, T z)
            : x(x), y(y), z(z)
        {}

        constexpr Quaternion(T w, T x, T y, T z)
            : w(w), x(x), y(y), z(z)
        {}

        explicit constexpr Quaternion(const Vector3<T>& axis, T angle)
            : w((T) cos_d(angle * 0.5))
        {
            reinterpret_cast<Vector3<T>&>(x) = normalize(axis) * (T) sin_d(angle * 0.5);
        }

        constexpr T& operator[](int index)
        {
            return (&w)[index];
        }

        constexpr const T& operator[](int index) const
        {
            return (&w)[index];
        }

        constexpr operator Vector3<T>() const
        {
            return { x, y, z };
        }

        constexpr operator Vector4<T>() const
        {
            return { x, y, z, w };
        }

        constexpr bool operator==(const Quaternion<T>& other) const
        {
            return w == other.w && x == other.x && y == other.y && z == other.z;
        }

        constexpr bool operator!=(const Quaternion<T>& other) const
        {
			return !(*this == other);
        }

        constexpr Quaternion<T> operator+(const Quaternion<T>& other) const
        {
			return { w + other.w, x + other.x, y + other.y, z + other.z };
        }

        constexpr void operator+=(const Quaternion<T>& other)
        {
			w += other.w;
			x += other.x;
			y += other.y;
			z += other.z;
        }

        constexpr Quaternion<T> operator-(const Quaternion<T>& other) const
        {
			return { w - other.w, x - other.x, y - other.y, z - other.z };
        }

        constexpr void operator-=(const Quaternion<T>& other)
        {
			w -= other.w;
			x -= other.x;
			y -= other.y;
			z -= other.z;
        }

        constexpr Quaternion<T> operator*(T value) const
        {
			return { w * value, x * value, y * value, z * value };
        }

        constexpr void operator*=(T value)
        {
			w *= value;
			x *= value;
			y *= value;
			z *= value;
        }

        constexpr Quaternion<T> operator*(const Quaternion<T>& other) const
        {
            return {
                w * other.w - x * other.x - y * other.y - z * other.z,
                w * other.x + x * other.w + y * other.z - z * other.y,
                w * other.y - x * other.z + y * other.w + z * other.x,
                w * other.z + x * other.y - y * other.x + z * other.w,
            };
        }

        constexpr void operator*=(const Quaternion<T>& other)
        {
            *this = *this * other;
        }

        constexpr Quaternion<T> operator-() const
        {
			return { -w, -x, -y, -z };
        }

        constexpr T length() const
        {
			return (T) std::sqrt(w * w + x * x + y * y + z * z);
        }
    };
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Quaternion<T>& quat)
    {
        stream << std::setprecision(2);
        stream << "(" << quat.w << " + " << quat.x << "i + " << quat.y << "j + " << quat.z << "k)";
        return stream;
    }
}
