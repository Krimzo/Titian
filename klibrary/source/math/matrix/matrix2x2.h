#pragma once

#include "math/vector/vector4.h"


namespace kl {
    template<typename T>
    struct Matrix2x2
    {
        T data[4] = {
            T(1), T(0),
            T(0), T(1),
        };

        constexpr Matrix2x2()
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
            return data[x + y * 2];
        }

        constexpr const T& operator()(int x, int y) const
        {
            return data[x + y * 2];
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
        constexpr operator Matrix2x2<O>() const
        {
			Matrix2x2<O> result;
			for (int i = 0; i < 4; i++) {
				result[i] = O(data[i]);
			}
			return result;
        }

        constexpr bool operator==(const Matrix2x2<T>& other) const
        {
			for (int i = 0; i < 4; i++) {
				if (data[i] != other[i]) {
					return false;
				}
			}
			return true;
        }

        constexpr bool operator!=(const Matrix2x2<T>& other) const
        {
			return !(*this == other);
        }

        constexpr Matrix2x2<T> operator+(const Matrix2x2<T>& other) const
        {
            Matrix2x2<T> result;
            for (int i = 0; i < 4; i++) {
                result[i] = data[i] + other[i];
            }
            return result;
        }

        constexpr void operator+=(const Matrix2x2<T>& other)
        {
			for (int i = 0; i < 4; i++) {
				data[i] += other[i];
			}
        }

        constexpr Matrix2x2<T> operator-(const Matrix2x2<T>& other) const
        {
			Matrix2x2<T> result;
			for (int i = 0; i < 4; i++) {
				result[i] = data[i] - other[i];
			}
			return result;
        }

        constexpr void operator-=(const Matrix2x2<T>& other)
        {
			for (int i = 0; i < 4; i++) {
				data[i] -= other[i];
			}
        }

        constexpr Matrix2x2<T> operator*(T value) const
        {
			Matrix2x2<T> result;
			for (int i = 0; i < 4; i++) {
				result[i] = data[i] * value;
			}
			return result;
        }

        constexpr void operator*=(T value)
        {
			for (int i = 0; i < 4; i++) {
				data[i] *= value;
			}
        }

        constexpr Matrix2x2<T> operator*(const Matrix2x2<T>& other) const
        {
            Matrix2x2<T> result;
            for (int y = 0; y < 2; y++) {
                for (int x = 0; x < 2; x++) {
                    result(x, y) = T(0);
                    for (int i = 0; i < 2; i++) {
                        result(x, y) += (*this)(i, y) * other(x, i);
                    }
                }
            }
            return result;
        }

        constexpr void operator*=(const Matrix2x2<T>& other)
        {
            *this = *this * other;
        }

        constexpr Vector2<T> operator*(const Vector2<T>& vec) const
        {
            Vector2<T> result;
            for (int y = 0; y < 2; y++) {
                for (int i = 0; i < 2; i++) {
                    result[y] += (*this)(i, y) * vec[i];
                }
            }
            return result;
        }

        constexpr T determinant() const
        {
            const T result = data[0] * data[3] - data[1] * data[2];
            return T(1) / result;
        }
    };
}

namespace kl {
    using Int2x2 = Matrix2x2<int32_t>;
    using UInt2x2 = Matrix2x2<uint32_t>;
    using Float2x2 = Matrix2x2<float>;
    using Double2x2 = Matrix2x2<double>;
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Matrix2x2<T>& mat)
    {
        stream << format_matrix<2, 2, T>(mat.data);
        return stream;
    }
}
