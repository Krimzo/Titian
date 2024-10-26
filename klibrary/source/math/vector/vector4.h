#pragma once

#include "math/vector/vector3.h"


namespace kl {
	template<typename T>
	struct Vector4
	{
		T x = {};
		T y = {};
		T z = {};
		T w = {};

		constexpr Vector4()
		{}

		explicit constexpr Vector4(T value)
			: x(value), y(value), z(value), w(value)
		{}

		constexpr Vector4(T x, T y, T z, T w)
			: x(x), y(y), z(z), w(w)
		{}

		constexpr Vector4(const Vector2<T>& vec, T z, T w)
			: x(vec.x), y(vec.y), z(z), w(w)
		{}

		constexpr Vector4(T x, const Vector2<T>& vec, T w)
			: x(x), y(vec.x), z(vec.y), w(w)
		{}

		constexpr Vector4(T x, T y, const Vector2<T>& vec)
			: x(x), y(y), z(vec.x), w(vec.y)
		{}

		constexpr Vector4(const Vector2<T>& vec1, const Vector2<T>& vec2)
			: x(vec1.x), y(vec1.y), z(vec2.x), w(vec2.y)
		{}

		constexpr Vector4(const Vector3<T>& vec, T w)
			: x(vec.x), y(vec.y), z(vec.z), w(w)
		{}

		constexpr Vector4(T x, const Vector3<T>& vec)
			: x(x), y(vec.x), z(vec.y), w(vec.z)
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
		constexpr operator Vector4<O>() const
		{
			return { O(x), O(y), O(z), O(w) };
		}

		constexpr operator Quaternion_T<T>() const
		{
			return { w, x, y, z };
		}

		constexpr operator RGB() const
		{
			return {
				(byte) clamp(x * to_byte_rgb<T>(), T(0), T(255)),
				(byte) clamp(y * to_byte_rgb<T>(), T(0), T(255)),
				(byte) clamp(z * to_byte_rgb<T>(), T(0), T(255)),
				(byte) clamp(w * to_byte_rgb<T>(), T(0), T(255)),
			};
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

		constexpr Vector2<T>& zw()
		{
			return reinterpret_cast<Vector2<T>&>(z);
		}

		constexpr const Vector2<T>& zw() const
		{
			return reinterpret_cast<const Vector2<T>&>(z);
		}

		constexpr Vector3<T>& xyz()
		{
			return reinterpret_cast<Vector3<T>&>(x);
		}

		constexpr const Vector3<T>& xyz() const
		{
			return reinterpret_cast<const Vector3<T>&>(x);
		}

		constexpr Vector3<T>& yzw()
		{
			return reinterpret_cast<Vector3<T>&>(y);
		}

		constexpr const Vector3<T>& yzw() const
		{
			return reinterpret_cast<const Vector3<T>&>(y);
		}

		constexpr bool operator==(const Vector4<T>& other) const
		{
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}

		constexpr bool operator!=(const Vector4<T>& other) const
		{
			return !(*this == other);
		}

		constexpr Vector4<T> operator+(const Vector4<T>& other) const
		{
			return { x + other.x, y + other.y, z + other.z, w + other.w };
		}

		constexpr void operator+=(const Vector4<T>& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
		}

		constexpr Vector4<T> operator-(const Vector4<T>& other) const
		{
			return { x - other.x, y - other.y, z - other.z, w - other.w };
		}

		constexpr void operator-=(const Vector4<T>& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
		}

		constexpr Vector4<T> operator*(T value) const
		{
			return { x * value, y * value, z * value, w * value };
		}

		constexpr void operator*=(T value)
		{
			x *= value;
			y *= value;
			z *= value;
			w *= value;
		}

		constexpr Vector4<T> operator*(const Vector4<T>& other) const
		{
			return { x * other.x, y * other.y, z * other.z, w * other.w };
		}

		constexpr void operator*=(const Vector4<T>& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
		}

		constexpr Vector4<T> operator/(T value) const
		{
			return { x / value, y / value, z / value, w / value };
		}

		constexpr void operator/=(T value)
		{
			x /= value;
			y /= value;
			z /= value;
			w /= value;
		}

		constexpr Vector4<T> operator/(const Vector4<T>& other) const
		{
			return { x / other.x, y / other.y, z / other.z, w / other.w };
		}

		constexpr void operator/=(const Vector4<T>& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
		}

		constexpr Vector4<T> operator-() const
		{
			return { -x, -y, -z, -w };
		}

		constexpr T length() const
		{
			return sqrt(x * x + y * y + z * z + w * w);
		}
	};
}

namespace kl {
	using Int4 = Vector4<int32_t>;
	using UInt4 = Vector4<uint32_t>;
	using Float4 = Vector4<float>;
	using Double4 = Vector4<double>;
}

namespace kl {
	template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Vector4<T>& vec)
	{
		stream << std::setprecision(2);
		stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
		return stream;
	}
}
