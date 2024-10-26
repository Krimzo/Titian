#pragma once

#include "math/vector/vector4.h"


namespace kl {
    template<typename T>
    struct Complex_T
    {
        T r = {};
        T i = {};

        constexpr Complex_T()
        {}

        constexpr Complex_T(T r, T i)
            : r(r), i(i)
        {}

        explicit constexpr Complex_T(T angle)
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

        constexpr bool operator==(const Complex_T<T>& other) const
        {
            return r == other.r && i == other.i;
        }

        constexpr bool operator!=(const Complex_T<T>& other) const
        {
            return !(*this == other);
        }

        constexpr Complex_T<T> operator+(const Complex_T<T>& other) const
        {
            return { r + other.r, i + other.i };
        }

        constexpr void operator+=(const Complex_T<T>& other)
        {
            r += other.r;
            i += other.i;
        }

        constexpr Complex_T<T> operator-(const Complex_T<T>& other) const
        {
            return { r - other.r, i - other.i };
        }

        constexpr void operator-=(const Complex_T<T>& other)
        {
            r -= other.r;
            i -= other.i;
        }

        constexpr Complex_T<T> operator*(T value) const
        {
            return { r * value, i * value };
        }

        constexpr void operator*=(T value)
        {
            r *= value;
            i *= value;
        }

        constexpr Complex_T<T> operator*(const Complex_T<T>& other) const
        {
            return {
                r * other.r - i * other.i,
                r * other.i + i * other.r,
            };
        }

        constexpr void operator*=(const Complex_T<T>& other)
        {
            *this = *this * other;
        }

        constexpr Complex_T<T> operator-() const
        {
            return { -r, -i };
        }

        constexpr T length() const
        {
            return sqrt(r * r + i * i);
        }
    };
}

namespace kl {
    using Complex = Complex_T<float>;
	using ComplexD = Complex_T<double>;
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Complex_T<T>& complex)
    {
        stream << std::setprecision(2);
        stream << "(" << complex.r << " + " << complex.i << "i)";
        return stream;
    }
}
