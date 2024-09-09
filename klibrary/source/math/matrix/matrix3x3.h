#pragma once

#include "math/matrix/matrix2x2.h"


namespace kl {
    template<typename T>
    struct Matrix3x3
    {
        T data[9] = {
            T(1), T(0), T(0),
            T(0), T(1), T(0),
            T(0), T(0), T(1),
        };

        constexpr Matrix3x3()
        {}

        constexpr T& operator[](int index)
		{
			return data[index];
		}

        constexpr const T& operator[](int index) const
		{
			return data[index];
		}

        constexpr T& operator()(int x, int y)
		{
			return data[x + y * 3];
		}

        constexpr const T& operator()(int x, int y) const
		{
			return data[x + y * 3];
		}

		constexpr T get(int index) const
		{
			return data[index];
		}

		constexpr void set(int index, T value)
		{
			data[index] = value;
		}

        template<typename O>
        constexpr operator Matrix3x3<O>() const
        {
            Matrix3x3<O> result;
            for (int i = 0; i < 9; i++) {
                result[i] = O(data[i]);
            }
            return result;
        }

        constexpr bool operator==(const Matrix3x3<T>& other) const
		{
			for (int i = 0; i < 9; i++) {
				if (data[i] != other[i]) {
					return false;
				}
			}
			return true;
		}

        constexpr bool operator!=(const Matrix3x3<T>& other) const
		{
			return !(*this == other);
		}

        constexpr Matrix3x3<T> operator+(const Matrix3x3<T>& other) const
		{
			Matrix3x3<T> result;
			for (int i = 0; i < 9; i++) {
				result[i] = data[i] + other[i];
			}
			return result;
		}

        constexpr void operator+=(const Matrix3x3<T>& other)
		{
			for (int i = 0; i < 9; i++) {
				data[i] += other[i];
			}
		}

        constexpr Matrix3x3<T> operator-(const Matrix3x3<T>& other) const
		{
			Matrix3x3<T> result;
			for (int i = 0; i < 9; i++) {
				result[i] = data[i] - other[i];
			}
			return result;
		}

        constexpr void operator-=(const Matrix3x3<T>& other)
		{
			for (int i = 0; i < 9; i++) {
				data[i] -= other[i];
			}
		}

        constexpr Matrix3x3<T> operator*(T value) const
		{
			Matrix3x3<T> result;
			for (int i = 0; i < 9; i++) {
				result[i] = data[i] * value;
			}
			return result;
		}

        constexpr void operator*=(T value)
		{
			for (int i = 0; i < 9; i++) {
				data[i] *= value;
			}
		}

        constexpr Matrix3x3<T> operator*(const Matrix3x3<T>& other) const
		{
			Matrix3x3<T> result;
			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < 3; x++) {
					result(x, y) = T(0);
					for (int i = 0; i < 3; i++) {
						result(x, y) += (*this)(i, y) * other(x, i);
					}
				}
			}
			return result;
		}

        constexpr void operator*=(const Matrix3x3<T>& other)
		{
			*this = *this * other;
		}

		constexpr Vector3<T> operator*(const Vector3<T>& vec) const
		{
			Vector3<T> result;
			for (int y = 0; y < 3; y++) {
				for (int i = 0; i < 3; i++) {
					result[y] += (*this)(i, y) * vec[i];
				}
			}
			return result;
		}

		constexpr T determinant() const
		{
			const T result =
				(*this)(0, 0) * ((*this)(1, 1) * (*this)(2, 2) - (*this)(2, 1) * (*this)(1, 2)) -
				(*this)(0, 1) * ((*this)(1, 0) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 0)) +
				(*this)(0, 2) * ((*this)(1, 0) * (*this)(2, 1) - (*this)(1, 1) * (*this)(2, 0));
			return T(1) / result;
		}

		static constexpr Matrix3x3<T> scaling(const Vector2<T>& scale)
		{
			Matrix3x3<T> result;
			result[0] = scale.x;
			result[4] = scale.y;
			return result;
		}

		static constexpr Matrix3x3<T> rotation(T rotation)
		{
			const T z_sin = sin_d(rotation);
			const T z_cos = cos_d(rotation);

			Matrix3x3<T> result;
			result[0] = z_cos;
			result[1] = -z_sin;
			result[3] = z_sin;
			result[4] = z_cos;
			return result;
		}

        static constexpr Matrix3x3<T> translation(const Vector2<T>& translation)
		{
			Matrix3x3<T> result;
			result[2] = translation.x;
			result[5] = translation.y;
			return result;
		}
    };
}

namespace kl {
	using Int3x3 = Matrix3x3<int32_t>;
	using UInt3x3 = Matrix3x3<uint32_t>;
	using Float3x3 = Matrix3x3<float>;
	using Double3x3 = Matrix3x3<double>;
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Matrix3x3<T>& mat)
    {
		stream << format_matrix<3, 3, T>(mat.data);
        return stream;
    }
}
