#pragma once

#include "render/vertex.h"


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

        triangle();
        triangle(const vertex& a, const vertex& b, const vertex& c);

        [[nodiscard]] float4 get_constants() const;

        [[nodiscard]] float3 get_weights(const float4& constants, const float2& point) const;
        [[nodiscard]] float3 get_weights(const float3& position) const;

        [[nodiscard]] static bool is_in_triangle(const float3& weights);

        [[nodiscard]] static float interpolate(const float3& weights, const float3& values);
        [[nodiscard]] vertex interpolate(const float3& weights) const;
    };
    
    std::ostream& operator<<(std::ostream& os, const triangle& obj);
}
