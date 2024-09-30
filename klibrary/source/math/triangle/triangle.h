#pragma once

#include "math/triangle/vertex.h"


namespace kl {
    template<typename T = float>
    struct Triangle
    {
        Vertex<T> a;
        Vertex<T> b;
        Vertex<T> c;

        constexpr Triangle()
        {}

        constexpr Triangle(const Vertex<T>& a, const Vertex<T>& b, const Vertex<T>& c)
			: a(a), b(b), c(c)
        {}

        constexpr Vector3<T> normal() const
        {
            return normalize(cross(b.world - a.world, c.world - a.world));
        }

        constexpr Vector4<T> constants() const
        {
            const T calc_const = (b.world.y - c.world.y) * (a.world.x - c.world.x) + (c.world.x - b.world.x) * (a.world.y - c.world.y);
            const T rec_constant = T(1) / calc_const;
            return {
                (b.world.y - c.world.y) * rec_constant,
                (c.world.x - b.world.x) * rec_constant,
                (c.world.y - a.world.y) * rec_constant,
                (a.world.x - c.world.x) * rec_constant,
            };
        }

        constexpr Vector3<T> weights(const Vector4<T>& constants, const Vector2<T>& point) const
        {
            const T dx = point.x - c.world.x;
            const T dy = point.y - c.world.y;
            const T w1 = dx * constants.x + dy * constants.y;
            const T w2 = dx * constants.z + dy * constants.w;
            return { w1, w2, T(1) - w1 - w2 };
        }

        constexpr Vector3<T> weights(const Vector3<T>& position) const
        {
            const Vector3<T> v0 = a.world - c.world;
            const Vector3<T> v1 = b.world - c.world;
            const Vector3<T> v2 = position - c.world;

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

        constexpr Vertex<T> interpolate_self(const Vector3<T>& weights) const
        {
            return Vertex<T>{
                Vector3<T>{
                    interpolate(weights, { a.world.x, b.world.x, c.world.x }),
                    interpolate(weights, { a.world.y, b.world.y, c.world.y }),
                    interpolate(weights, { a.world.z, b.world.z, c.world.z }),
                },
                Vector2<T>{
                    interpolate(weights, { a.texture.x, b.texture.x, c.texture.x }),
                    interpolate(weights, { a.texture.y, b.texture.y, c.texture.y }),
                },
                Vector3<T>{
                    interpolate(weights, { a.normal.x, b.normal.x, c.normal.x }),
                    interpolate(weights, { a.normal.y, b.normal.y, c.normal.y }),
                    interpolate(weights, { a.normal.z, b.normal.z, c.normal.z }),
                }
            };
        }
    };
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Triangle<T>& triangle)
    {
        stream << "{" << triangle.a << ", " << triangle.b << ", " << triangle.c << "}";
        return stream;
    }
}
