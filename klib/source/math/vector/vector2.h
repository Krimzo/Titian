#pragma once

#include <iomanip>


namespace kl::math {
    inline constexpr float pi = 3.14159265358979f;

    template <typename T>
    T to_radians(const T& degrees)
    {
        static constexpr float conversion = pi / 180.0f;
        return T(degrees * conversion);
    }

    template <typename T>
    T to_degrees(const T& radians)
    {
        static constexpr float conversion = 180.0f / pi;
        return T(radians * conversion);
    }
}

namespace kl {
    template <typename T>
    struct vector2
    {
        union
        {
            struct
            {
                T x, y;
            };

            struct
            {
                T u, v;
            };

            struct
            {
                T width, height;
            };

            T data[2] = {};
        };

        vector2()
        {}

        template <typename T0, typename T1>
        vector2(const T0& x, const T1& y)
            : x(T(x)), y(T(y))
        {}

        template <typename T0>
        static vector2<T> splash(const T0& value)
        {
            return { value, value };
        }

        T& operator[](int ind)
        {
            return data[ind];
        }

        const T& operator[](int ind) const
        {
            return data[ind];
        }

        template <typename T0>
        explicit operator vector2<T0>() const
        {
            return { T0(x), T0(y) };
        }

        vector2<T> operator+(const vector2<T>& obj) const
        {
            vector2<T> result;
            for (int i = 0; i < 2; i++) {
                result[i] = data[i] + obj[i];
            }
            return result;
        }

        void operator+=(const vector2<T>& obj)
        {
            *this = *this + obj;
        }

        vector2<T> operator-(const vector2<T>& obj) const
        {
            vector2<T> result;
            for (int i = 0; i < 2; i++) {
                result[i] = data[i] - obj[i];
            }
            return result;
        }

        void operator-=(const vector2<T>& obj)
        {
            *this = *this - obj;
        }

        vector2<T> operator*(const T& val) const
        {
            vector2<T> result;
            for (int i = 0; i < 2; i++) {
                result[i] = data[i] * val;
            }
            return result;
        }

        void operator*=(const T& val)
        {
            *this = *this * val;
        }

        vector2<T> operator*(const vector2<T>& obj) const
        {
            vector2<T> result;
            for (int i = 0; i < 2; i++) {
                result[i] = data[i] * obj[i];
            }
            return result;
        }

        void operator*=(const vector2<T>& obj)
        {
            *this = *this * obj;
        }

        vector2<T> operator/(const T& val) const
        {
            vector2<T> result;
            for (int i = 0; i < 2; i++) {
                result[i] = data[i] / val;
            }
            return result;
        }

        void operator/=(const T& val)
        {
            *this = *this / val;
        }

        vector2<T> operator/(const vector2<T>& obj) const
        {
            vector2<T> result;
            for (int i = 0; i < 2; i++) {
                result[i] = data[i] / obj[i];
            }
            return result;
        }

        void operator/=(const vector2<T>& obj)
        {
            *this = *this / obj;
        }

        bool operator==(const vector2<T>& obj) const
        {
            for (int i = 0; i < 2; i++) {
                if (data[i] != obj[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const vector2<T>& obj) const
        {
            return !(*this == obj);
        }

        [[nodiscard]] vector2<T> absolute() const
        {
            vector2<T> result;
            for (int i = 0; i < 2; i++) {
                result[i] = std::abs(data[i]);
            }
            return result;
        }

        [[nodiscard]] vector2<T> negate() const
        {
            return *this * -1.0f;
        }

        [[nodiscard]] T length() const
        {
            T sum = {};
            for (int i = 0; i < 2; i++) {
                sum += data[i] * data[i];
            }
            return std::sqrt(sum);
        }

        [[nodiscard]] vector2<T> normalize() const
        {
            return *this / length();
        }

        [[nodiscard]] T dot(const vector2<T>& vec) const
        {
            T sum = {};
            for (int i = 0; i < 2; i++) {
                sum += data[i] * vec[i];
            }
            return sum;
        }

        [[nodiscard]] T angle(const vector2<T>& vec, bool full = false) const
        {
            if (full) {
                return math::to_degrees(std::atan2(x * vec.y - y * vec.x, x * vec.x + y * vec.y));
            }
            return math::to_degrees(std::acos(normalize().dot(vec.normalize())));
        }

        [[nodiscard]] vector2<T> rotate(const T& angle) const
        {
            const T sin_a = T(std::sin(math::to_radians(angle)));
            const T cos_a = T(std::cos(math::to_radians(angle)));
            return { cos_a * x - sin_a * y, sin_a * x + cos_a * y };
        }

        [[nodiscard]] vector2<T> reflect(const vector2<T>& normal) const
        {
            vector2<T> normalized = normal.normalize();
            return *this - normalized * dot(normalized) * 2.0f;
        }
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& stream, const vector2<T>& obj)
    {
        stream << std::fixed << std::setprecision(2);
        stream << "(" << obj.x << ", " << obj.y << ")";
        return stream;
    }
}

namespace kl {
    using float2 = vector2<float>;
    using int2 = vector2<int>;

    using vec2 = float2;
    using ivec2 = int2;
}
