#pragma once

#include "math/vector/vector4.h"


namespace kl {
    template<typename T = float>
    struct Complex
    {
        T r = {};
        T i = {};

        constexpr Complex()
        {}

        constexpr Complex(T r, T i)
            : r(r), i(i)
        {}

        explicit constexpr Complex(T angle)
            : r(cos_d(angle)), i(sin_d(angle))
        {}

        constexpr T& operator[](int index)
        {
            return (&r)[index];
        }

        constexpr const T& operator[](int index) const
        {
            return (&r)[index];
        }

        constexpr operator Vector2<T>() const
        {
            return { r, i };
        }

        constexpr bool operator==(const Complex<T>& other) const
        {
            return r == other.r && i == other.i;
        }

        constexpr bool operator!=(const Complex<T>& other) const
        {
            return !(*this == other);
        }

        constexpr Complex<T> operator+(const Complex<T>& other) const
        {
            return { r + other.r, i + other.i };
        }

        constexpr void operator+=(const Complex<T>& other)
        {
            r += other.r;
            i += other.i;
        }

        constexpr Complex<T> operator-(const Complex<T>& other) const
        {
            return { r - other.r, i - other.i };
        }

        constexpr void operator-=(const Complex<T>& other)
        {
            r -= other.r;
            i -= other.i;
        }

        constexpr Complex<T> operator*(T value) const
        {
            return { r * value, i * value };
        }

        constexpr void operator*=(T value)
        {
            r *= value;
            i *= value;
        }

        constexpr Complex<T> operator*(const Complex<T>& other) const
        {
            return {
                r * other.r - i * other.i,
                r * other.i + i * other.r,
            };
        }

        constexpr void operator*=(const Complex<T>& other)
        {
            *this = *this * other;
        }

        constexpr Complex<T> operator-() const
        {
            return { -r, -i };
        }

        constexpr T length() const
        {
            return (T) std::sqrt(r * r + i * i);
        }
    };
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Complex<T>& complex)
    {
        stream << std::setprecision(2);
        stream << "(" << complex.r << " + " << complex.i << "i)";
        return stream;
    }
}
