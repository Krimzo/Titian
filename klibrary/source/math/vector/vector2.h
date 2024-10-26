#pragma once

#include "math/basic/basic.h"
#include "media/image/color.h"


namespace kl {
    template<typename T>
    struct Complex_T;
}

namespace kl {
    template<typename T>
    struct Vector2
    {
        T x = {};
        T y = {};

        constexpr Vector2()
        {}

        explicit constexpr Vector2(T value)
			: x(value), y(value)
        {}

        constexpr Vector2(T x, T y)
			: x(x), y(y)
        {}

		constexpr T& operator[](int index)
		{
			return (&x)[index];
		}

		constexpr const T& operator[](int index) const
		{
			return (&x)[index];
		}
        
        template<typename O>
        constexpr operator Vector2<O>() const
		{
			return { O(x), O(y) };
		}

        constexpr operator Complex_T<T>() const
		{
			return { x, y };
		}

        constexpr bool operator==(const Vector2<T>& other) const
		{
			return x == other.x && y == other.y;
		}

        constexpr bool operator!=(const Vector2<T>& other) const
		{
			return !(*this == other);
		}

		constexpr Vector2<T> operator+(const Vector2<T>& other) const
		{
			return { x + other.x, y + other.y };
		}

		constexpr void operator+=(const Vector2<T>& other)
		{
			x += other.x;
			y += other.y;
		}

		constexpr Vector2<T> operator-(const Vector2<T>& other) const
		{
			return { x - other.x, y - other.y };
		}

		constexpr void operator-=(const Vector2<T>& other)
		{
			x -= other.x;
			y -= other.y;
		}

		constexpr Vector2<T> operator*(T value) const
		{
			return { x * value, y * value };
		}

		constexpr void operator*=(T value)
		{
			x *= value;
			y *= value;
		}

		constexpr Vector2<T> operator*(const Vector2<T>& other) const
		{
			return { x * other.x, y * other.y };
		}

		constexpr void operator*=(const Vector2<T>& other)
		{
			x *= other.x;
			y *= other.y;
		}

		constexpr Vector2<T> operator/(T value) const
		{
			return { x / value, y / value };
		}

		constexpr void operator/=(T value)
		{
			x /= value;
			y /= value;
		}

		constexpr Vector2<T> operator/(const Vector2<T>& other) const
		{
			return { x / other.x, y / other.y };
		}

		constexpr void operator/=(const Vector2<T>& other)
		{
			x /= other.x;
			y /= other.y;
		}

		constexpr Vector2<T> operator-() const
		{
			return { -x, -y };
		}

		constexpr T length() const
		{
			return sqrt(x * x + y * y);
		}

		constexpr bool in_bounds(const Vector2<T>& lower_incl, const Vector2<T>& upper_excl) const
		{
			return x >= lower_incl.x && x < upper_excl.x
				&& y >= lower_incl.y && y < upper_excl.y;
		}

		constexpr bool in_bounds(const Vector2<T>& upper_excl) const
		{
			return in_bounds({}, upper_excl);
		}

		constexpr T to_index(T width) const
		{
			return x + y * width;
		}

		static constexpr Vector2<T> from_index(T index, T width)
		{
			return { index % width, index / width };
		}
    };
}

namespace kl {
	using Int2 = Vector2<int32_t>;
	using UInt2 = Vector2<uint32_t>;
	using Float2 = Vector2<float>;
	using Double2 = Vector2<double>;
}

namespace kl {
	template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Vector2<T>& vec)
	{
		stream << std::setprecision(2);
		stream << "(" << vec.x << ", " << vec.y << ")";
		return stream;
	}
}
