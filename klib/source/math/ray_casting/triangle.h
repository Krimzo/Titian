#pragma once

#include "math/ray_casting/vertex.h"


namespace kl {
    struct triangle
    {
        union
        {
            struct
            {
                vertex a;
                vertex b;
                vertex c;
            };

            vertex data[3] = {};
        };

        triangle()
        {}

        triangle(const vertex& a, const vertex& b, const vertex& c)
            : a(a), b(b), c(c)
        {}

        [[nodiscard]] float4 get_constants() const
        {
            const float calc_const = (b.world.y - c.world.y) * (a.world.x - c.world.x) + (c.world.x - b.world.x) * (a.world.y - c.world.y);
            const float rec_constant = 1.0f / calc_const;
            return {
                (b.world.y - c.world.y) * rec_constant,
                (c.world.x - b.world.x) * rec_constant,
                (c.world.y - a.world.y) * rec_constant,
                (a.world.x - c.world.x) * rec_constant
            };
        }

        [[nodiscard]] float3 get_weights(const float4& constants, const float2& point) const
        {
            const float dx = point.x - c.world.x;
            const float dy = point.y - c.world.y;
            const float w1 = dx * constants.x + dy * constants.y;
            const float w2 = dx * constants.z + dy * constants.w;
            return { w1, w2, (1.0f - w1 - w2) };
        }

        [[nodiscard]] float3 get_weights(const float3& position) const
        {
            const float3 v0 = a.world - c.world;
            const float3 v1 = b.world - c.world;
            const float3 v2 = position - c.world;

            const float d00 = v0.dot(v0);
            const float d01 = v0.dot(v1);
            const float d11 = v1.dot(v1);
            const float d20 = v2.dot(v0);
            const float d21 = v2.dot(v1);

            const float inverse_denom = 1.0f / (d00 * d11 - d01 * d01);
            const float w1 = (d11 * d20 - d01 * d21) * inverse_denom;
            const float w2 = (d00 * d21 - d01 * d20) * inverse_denom;

            return { w1, w2, (1.0f - w1 - w2) };
        }

        [[nodiscard]] bool is_in_triangle(const float3& weights)
        {
            return !(weights.x < 0.0f || weights.y < 0.0f || weights.z < 0.0f);
        }

        [[nodiscard]] float interpolate(const float3& weights, const float3& values) const
        {
            return weights.dot(values);
        }

        [[nodiscard]] vertex interpolate(const float3& weights) const
        {
            return {
                float3{
                    interpolate(weights, float3{ a.world.x, b.world.x, c.world.x }),
                    interpolate(weights, float3{ a.world.y, b.world.y, c.world.y }),
                    interpolate(weights, float3{ a.world.z, b.world.z, c.world.z })
                },
                float2{
                    interpolate(weights, float3{ a.texture.x, b.texture.x, c.texture.x }),
                    interpolate(weights, float3{ a.texture.y, b.texture.y, c.texture.y })
                },
                float3{
                    interpolate(weights, float3{ a.normal.x, b.normal.x, c.normal.x }),
                    interpolate(weights, float3{ a.normal.y, b.normal.y, c.normal.y }),
                    interpolate(weights, float3{ a.normal.z, b.normal.z, c.normal.z })
                }
            };
        }

        friend std::ostream& operator<<(std::ostream& os, const triangle& obj)
        {
            os << "{" << obj.a << ", " << obj.b << ", " << obj.c << "}";
            return os;
        }
    };
}
