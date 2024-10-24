#pragma once

#include "math/triangle/vertex.h"


namespace kl {
    template<typename T>
    struct Triangle_T
    {
        Vertex_T<T> a;
        Vertex_T<T> b;
        Vertex_T<T> c;

        constexpr Triangle_T()
        {}

        constexpr Triangle_T(const Vertex_T<T>& a, const Vertex_T<T>& b, const Vertex_T<T>& c)
			: a(a), b(b), c(c)
        {}

        constexpr Vector3<T> normal() const
        {
            return normalize(cross(b.position - a.position, c.position - a.position));
        }

        constexpr Vector4<T> constants() const
        {
            const T calc_const = (b.y - c.y) * (a.x - c.x)
                + (c.x - b.x) * (a.y - c.y);
            const T rec_constant = T(1) / calc_const;
            return {
                (b.y - c.y) * rec_constant,
                (c.x - b.x) * rec_constant,
                (c.y - a.y) * rec_constant,
                (a.x - c.x) * rec_constant,
            };
        }

        constexpr Vector3<T> weights(const Vector4<T>& constants, const Vector2<T>& point) const
        {
            const T dx = point.x - c.x;
            const T dy = point.y - c.y;
            const T w1 = dx * constants.x + dy * constants.y;
            const T w2 = dx * constants.z + dy * constants.w;
            return { w1, w2, T(1) - w1 - w2 };
        }

        constexpr Vector3<T> weights(const Vector3<T>& position) const
        {
            const Vector3<T> v0 = a.position - c.position;
            const Vector3<T> v1 = b.position - c.position;
            const Vector3<T> v2 = position - c.position;

            const T d00 = dot(v0, v0);
            const T d01 = dot(v0, v1);
            const T d11 = dot(v1, v1);
            const T d20 = dot(v2, v0);
            const T d21 = dot(v2, v1);

            const T inverse_denom = T(1) / (d00 * d11 - d01 * d01);
            const T w1 = (d11 * d20 - d01 * d21) * inverse_denom;
            const T w2 = (d00 * d21 - d01 * d20) * inverse_denom;
            return { w1, w2, T(1) - w1 - w2 };
        }

        static constexpr bool is_in_triangle(const Vector3<T>& weights)
        {
            return weights.x >= T(0) && weights.y >= T(0) && weights.z >= T(0);
        }

        static constexpr T interpolate(const Vector3<T>& weights, const Vector3<T>& values)
        {
            return dot(weights, values);
        }

        constexpr Vertex_T<T> interpolate_self(const Vector3<T>& weights) const
        {
            return Vertex_T<T>{
                Vector3<T>{
                    interpolate(weights, { a.x, b.x, c.x }),
                    interpolate(weights, { a.y, b.y, c.y }),
                    interpolate(weights, { a.z, b.z, c.z }),
                },
                normalize( Vector3<T>{
                    interpolate(weights, { a.normal.x, b.normal.x, c.normal.x }),
                    interpolate(weights, { a.normal.y, b.normal.y, c.normal.y }),
                    interpolate(weights, { a.normal.z, b.normal.z, c.normal.z }),
                } ),
                Vector2<T>{
                    interpolate(weights, { a.uv.x, b.uv.x, c.uv.x }),
                    interpolate(weights, { a.uv.y, b.uv.y, c.uv.y }),
                },
            };
        }
    };
}

namespace kl {
    using Triangle = Triangle_T<float>;
	using TriangleD = Triangle_T<double>;
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Triangle_T<T>& triangle)
    {
        stream << "{" << triangle.a << ", " << triangle.b << ", " << triangle.c << "}";
        return stream;
    }
}
