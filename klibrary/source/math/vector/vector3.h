#pragma once

#include "math/vector/vector2.h"


namespace kl {
    template<typename T>
    struct Quaternion_T;
}

namespace kl {
	template<typename T>
	struct Vector3
	{
		T x = {};
		T y = {};
		T z = {};

		constexpr Vector3()
		{}

		explicit constexpr Vector3(T value)
			: x(value), y(value), z(value)
		{}

		constexpr Vector3(T x, T y, T z)
			: x(x), y(y), z(z)
		{}

		constexpr Vector3(const Vector2<T>& vec, T z)
			: x(vec.x), y(vec.y), z(z)
		{}

		constexpr Vector3(T x, const Vector2<T>& vec)
			: x(x), y(vec.x), z(vec.y)
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
		constexpr operator Vector3<O>() const
		{
			return { O(x), O(y), O(z) };
		}

		constexpr operator Quaternion_T<T>() const
		{
			return { x, y, z };
		}

		constexpr operator RGB() const
		{
			return {
				(byte) clamp(x * to_byte_rgb<T>(), T(0), T(255)),
				(byte) clamp(y * to_byte_rgb<T>(), T(0), T(255)),
				(byte) clamp(z * to_byte_rgb<T>(), T(0), T(255)),
			};
		}

		constexpr operator YUV() const
		{
			return { x, y, z };
		}

		constexpr Vector2<T>& xy()
		{
			return reinterpret_cast<Vector2<T>&>(x);
		}

		constexpr const Vector2<T>& xy() const
		{
			return reinterpret_cast<const Vector2<T>&>(x);
		}

		constexpr Vector2<T>& yz()
		{
			return reinterpret_cast<Vector2<T>&>(y);
		}

		constexpr const Vector2<T>& yz() const
		{
			return reinterpret_cast<const Vector2<T>&>(y);
		}

		constexpr bool operator==(const Vector3<T>& other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}

		constexpr bool operator!=(const Vector3<T>& other) const
		{
			return !(*this == other);
		}

		constexpr Vector3<T> operator+(const Vector3<T>& other) const
		{
			return { x + other.x, y + other.y, z + other.z };
		}

		constexpr void operator+=(const Vector3<T>& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
		}

		constexpr Vector3<T> operator-(const Vector3<T>& other) const
		{
			return { x - other.x, y - other.y, z - other.z };
		}

		constexpr void operator-=(const Vector3<T>& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}

		constexpr Vector3<T> operator*(T value) const
		{
			return { x * value, y * value, z * value };
		}

		constexpr void operator*=(T value)
		{
			x *= value;
			y *= value;
			z *= value;
		}

		constexpr Vector3<T> operator*(const Vector3<T>& other) const
		{
			return { x * other.x, y * other.y, z * other.z };
		}

		constexpr void operator*=(const Vector3<T>& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
		}

		constexpr Vector3<T> operator/(T value) const
		{
			return { x / value, y / value, z / value };
		}

		constexpr void operator/=(T value)
		{
			x /= value;
			y /= value;
			z /= value;
		}

		constexpr Vector3<T> operator/(const Vector3<T>& other) const
		{
			return { x / other.x, y / other.y, z / other.z };
		}

		constexpr void operator/=(const Vector3<T>& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
		}

		constexpr Vector3<T> operator-() const
		{
			return { -x, -y, -z };
		}

		constexpr T length() const
		{
			return sqrt(x * x + y * y + z * z);
		}

		constexpr bool in_bounds(const Vector3<T>& lower_incl, const Vector3<T>& upper_excl) const
		{
			return x >= lower_incl.x && x < upper_excl.x
				&& y >= lower_incl.y && y < upper_excl.y
				&& z >= lower_incl.z && z < upper_excl.z;
		}

		constexpr bool in_bounds(const Vector3<T>& upper_excl) const
		{
			return in_bounds({}, upper_excl);
		}
	};
}

namespace kl {
	using Int3 = Vector3<int32_t>;
	using UInt3 = Vector3<uint32_t>;
	using Float3 = Vector3<float>;
	using Double3 = Vector3<double>;
}

namespace kl {
	template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Vector3<T>& vec)
	{
		stream << std::setprecision(2);
		stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return stream;
	}
}
