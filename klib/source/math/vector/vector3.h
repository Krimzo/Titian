#pragma once

#include "math/vector/vector2.h"
#include "media/image/color.h"

#include <algorithm>


namespace kl {
	inline float to_float_color(const uint8_t value)
	{
		static constexpr float conversion = 1.0f / 255.0f;
		return value * conversion;
	}

	inline uint8_t to_byte_color(const float value)
	{
		return uint8_t(std::min(std::max(int(value * 255), 0), 255));
	}
}

namespace kl {
	template<typename T>
	struct vector3
	{
		union
		{
			struct
			{
				T x, y, z;
			};

			struct
			{
				T r, g, b;
			};

			struct
			{
				T width, height, depth;
			};

			vector2<T> xy;
			vector2<T> rg;
			vector2<T> wh;

			T data[3] = {};
		};

		vector3()
		{}

		template<typename T0, typename T1, typename T2>
		vector3(const T0& x, const T1& y, const T2& z)
			: x(T(x)), y(T(y)), z(T(z))
		{}

		template<typename T0>
		vector3(const vector2<T>& v, const T0& z)
			: x(v.x), y(v.y), z(T(z))
		{}

		template<typename T0>
		vector3(const T0& x, const vector2<T>& v)
			: x(T(x)), y(v.x), z(v.y)
		{}

		explicit vector3(const color& color)
			: x(to_float_color(color.r)), y(to_float_color(color.g)), z(to_float_color(color.b))
		{}

		template<typename T0>
		static vector3<T> splash(const T0& value)
		{
			return { value, value, value };
		}

		T& operator[](int ind)
		{
			return data[ind];
		}

		const T& operator[](int ind) const
		{
			return data[ind];
		}

		template<typename T0>
		explicit operator vector3<T0>() const
		{
			return { T0(x), T0(y), T0(z) };
		}

		explicit operator color() const
		{
			return { to_byte_color(x), to_byte_color(y), to_byte_color(z) };
		}

		vector3<T> operator+(const vector3<T>& obj) const
		{
			vector3<T> result;
			for (int i = 0; i < 3; i++) {
				result[i] = data[i] + obj[i];
			}
			return result;
		}

		void operator+=(const vector3<T>& obj)
		{
			*this = *this + obj;
		}

		vector3<T> operator-(const vector3<T>& obj) const
		{
			vector3<T> result;
			for (int i = 0; i < 3; i++) {
				result[i] = data[i] - obj[i];
			}
			return result;
		}

		void operator-=(const vector3<T>& obj)
		{
			*this = *this - obj;
		}

		vector3<T> operator*(const T& val) const
		{
			vector3<T> result;
			for (int i = 0; i < 3; i++) {
				result[i] = data[i] * val;
			}
			return result;
		}

		void operator*=(const T& val)
		{
			*this = *this * val;
		}

		vector3<T> operator*(const vector3<T>& obj) const
		{
			vector3<T> result;
			for (int i = 0; i < 3; i++) {
				result[i] = data[i] * obj[i];
			}
			return result;
		}

		void operator*=(const vector3<T>& obj)
		{
			*this = *this * obj;
		}

		vector3<T> operator/(const T& val) const
		{
			vector3<T> result;
			for (int i = 0; i < 3; i++) {
				result[i] = data[i] / val;
			}
			return result;
		}

		void operator/=(const T& val)
		{
			*this = *this / val;
		}

		vector3<T> operator/(const vector3<T>& obj) const
		{
			vector3<T> result;
			for (int i = 0; i < 3; i++) {
				result[i] = data[i] / obj[i];
			}
			return result;
		}

		void operator/=(const vector3<T>& obj)
		{
			*this = *this / obj;
		}

		bool operator==(const vector3<T>& obj) const
		{
			for (int i = 0; i < 3; i++) {
				if (data[i] != obj[i]) {
					return false;
				}
			}
			return true;
		}

		bool operator!=(const vector3<T>& obj) const
		{
			return !(*this == obj);
		}

		[[nodiscard]] vector3<T> absolute() const
		{
			vector3<T> result;
			for (int i = 0; i < 3; i++) {
				result[i] = std::abs(data[i]);
			}
			return result;
		}

		[[nodiscard]] vector3<T> negate() const
		{
			return *this * -1.0f;
		}

		[[nodiscard]] T length() const
		{
			T sum = {};
			for (auto& i : data) {
				sum += i * i;
			}
			return std::sqrt(sum);
		}

		[[nodiscard]] vector3<T> normalize() const
		{
			return *this / length();
		}

		[[nodiscard]] T dot(const vector3<T>& vec) const
		{
			T sum = {};
			for (int i = 0; i < 3; i++) {
				sum += data[i] * vec[i];
			}
			return sum;
		}

		[[nodiscard]] vector3<T> cross(const vector3<T>& a) const
		{
			return { y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x };
		}

		[[nodiscard]] T angle(const vector3<T>& vec) const
		{
			return math::to_degrees(std::acos(normalize().dot(vec.normalize())));
		}

		[[nodiscard]] vector3<T> rotate(const T& angle, const vector3<T>& axis) const
		{
			const T angle_sin = T(std::sin(math::to_radians(angle) * 0.5f));
			const T angle_cos = T(std::cos(math::to_radians(angle) * 0.5f));
			const T qx = axis.x * angle_sin;
			const T qy = axis.y * angle_sin;
			const T qz = axis.z * angle_sin;
			const T x2 = qx * qx;
			const T y2 = qy * qy;
			const T z2 = qz * qz;
			const T w2 = angle_cos * angle_cos;
			const T xy = qx * qy;
			const T xz = qx * qz;
			const T yz = qy * qz;
			const T xw = qx * angle_cos;
			const T yw = qy * angle_cos;
			const T zw = qz * angle_cos;

			return {
				x * (w2 + x2 - z2 - y2) + y * (-zw + xy - zw + xy) + z * (yw + xz + xz + yw),
				x * (xy + zw + zw + xy) + y * (y2 - z2 + w2 - x2) + z * (yz + yz - xw - xw),
				x * (xz - yw + xz - yw) + y * (yz + yz + xw + xw) + z * (z2 - y2 - x2 + w2)
			};
		}

		[[nodiscard]] vector3<T> reflect(const vector3<T>& vec) const
		{
			const vector3<T> normal = vec.normalize();
			return *this - normal * dot(normal) * 2.0f;
		}
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& stream, const vector3<T>& obj)
	{
		stream << std::fixed << std::setprecision(2);
		stream << "(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
		return stream;
	}
}

namespace kl {
	using float3 = vector3<float>;
	using int3 = vector3<int>;

	using vec3 = float3;
	using ivec3 = int3;
}
