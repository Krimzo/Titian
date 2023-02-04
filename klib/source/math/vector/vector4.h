#pragma once

#include "math/vector/vector3.h"


namespace kl {
    template <typename T>
    struct vector4
    {
        union
        {
            struct
            {
                T x, y, z, w;
            };

            struct
            {
                T r, g, b, a;
            };

            vector2<T> xy;
            vector2<T> rg;

            vector3<T> xyz;
            vector3<T> rgb;

            T data[4] = {};
        };

        vector4()
        {}

        template <typename T0, typename T1, typename T2, typename T3>
        vector4(const T0& x, const T1& y, const T2& z, const T3& w)
            : x(T(x)), y(T(y)), z(T(z)), w(T(w))
        {}

        template <typename T0>
        vector4(const vector3<T>& v, const T0& w)
            : x(v.x), y(v.y), z(v.z), w(T(w))
        {}

        template <typename T0>
        vector4(const T0& x, const vector3<T>& v)
            : x(T(x)), y(v.x), z(v.y), w(v.z)
        {}

        template <typename T0, typename T1>
        vector4(const vector2<T>& v, const T0& z, const T1& w)
            : x(v.x), y(v.y), z(T(z)), w(T(w))
        {}

        template <typename T0, typename T1>
        vector4(const T0& x, const vector2<T>& v, const T1& w)
            : x(T(x)), y(v.x), z(v.y), w(T(w))
        {}

        template <typename T0, typename T1>
        vector4(const T0& x, const T1& y, const vector2<T>& v)
            : x(T(x)), y(T(y)), z(v.x), w(v.y)
        {}

        vector4(const vector2<T>& v1, const vector2<T>& v2)
            : x(v1.x), y(v1.y), z(v2.x), w(v2.y)
        {}

        explicit vector4(const color& color)
            : x(to_float_color(color.r)), y(to_float_color(color.g)), z(to_float_color(color.b)), w(to_float_color(color.a))
        {}

        template <typename T0>
        static vector4<T> splash(const T0& value)
        {
            return { value, value, value, value };
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
        explicit operator vector4<T0>() const
        {
            return { T0(x), T0(y), T0(z), T0(w) };
        }

        explicit operator color() const
        {
            return { to_byte_color(x), to_byte_color(y), to_byte_color(z), to_byte_color(w) };
        }

        vector4<T> operator+(const vector4<T>& obj) const
        {
            vector4<T> result;
            for (int i = 0; i < 4; i++) {
                result[i] = data[i] + obj[i];
            }
            return result;
        }

        void operator+=(const vector4<T>& obj)
        {
            *this = *this + obj;
        }

        vector4<T> operator-(const vector4<T>& obj) const
        {
            vector4<T> result;
            for (int i = 0; i < 4; i++) {
                result[i] = data[i] - obj[i];
            }
            return result;
        }

        void operator-=(const vector4<T>& obj)
        {
            *this = *this - obj;
        }

        vector4<T> operator*(const T& val) const
        {
            vector4<T> result;
            for (int i = 0; i < 4; i++) {
                result[i] = data[i] * val;
            }
            return result;
        }

        void operator*=(const T& val)
        {
            *this = *this * val;
        }

        vector4<T> operator*(const vector4<T>& obj) const
        {
            vector4<T> result;
            for (int i = 0; i < 4; i++) {
                result[i] = data[i] * obj[i];
            }
            return result;
        }

        void operator*=(const vector4<T>& obj)
        {
            *this = *this * obj;
        }

        vector4<T> operator/(const T& val) const
        {
            vector4<T> result;
            for (int i = 0; i < 4; i++) {
                result[i] = data[i] / val;
            }
            return result;
        }

        void operator/=(const T& val)
        {
            *this = *this / val;
        }

        vector4<T> operator/(const vector4<T>& obj) const
        {
            vector4<T> result;
            for (int i = 0; i < 4; i++) {
                result[i] = data[i] / obj[i];
            }
            return result;
        }

        void operator/=(const vector4<T>& obj)
        {
            *this = *this / obj;
        }

        bool operator==(const vector4<T>& obj) const
        {
            for (int i = 0; i < 4; i++) {
                if (data[i] != obj[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const vector4<T>& obj) const
        {
            return !(*this == obj);
        }

        [[nodiscard]] vector4<T> absolute() const
        {
            vector4<T> result;
            for (int i = 0; i < 4; i++) {
                result[i] = std::abs(data[i]);
            }
            return result;
        }

        [[nodiscard]] vector4<T> negate() const
        {
            return *this * -1.0f;
        }

        [[nodiscard]] T length() const
        {
            T sum = {};
            for (int i = 0; i < 4; i++) {
                sum += data[i] * data[i];
            }
            return std::sqrt(sum);
        }

        [[nodiscard]] vector4<T> normalize() const
        {
            return *this / length();
        }

        [[nodiscard]] T dot(const vector4<T>& vec) const
        {
            T sum = {};
            for (int i = 0; i < 4; i++) {
                sum += data[i] * vec[i];
            }
            return sum;
        }

        [[nodiscard]] T angle(const vector4<T>& vec) const
        {
            return math::to_degrees(std::acos(normalize().dot(vec.normalize())));
        }
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& stream, const vector4<T>& obj)
    {
        stream << std::fixed << std::setprecision(2);
        stream << "(" << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.w << ")";
        return stream;
    }
}

namespace kl {
    using float4 = vector4<float>;
    using int4 = vector4<int>;

    using vec4 = float4;
    using ivec4 = int4;
}
