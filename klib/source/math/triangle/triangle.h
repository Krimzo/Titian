#pragma once

#include "math/triangle/vertex.h"


namespace kl {
    struct triangle
    {
        vertex a = {};
        vertex b = {};
        vertex c = {};

        // Construct
        triangle();
        triangle(const vertex& a, const vertex& b, const vertex& c);

        // Normal
        float3 get_normal() const;

        // Interpolation
        float4 get_constants() const;
        float3 get_weights(const float4& constants, const float2& point) const;
        float3 get_weights(const float3& position) const;
        vertex interpolate_self(const float3& weights) const;

        // Static
        static bool is_in_triangle(const float3& weights);
        static float interpolate(const float3& weights, const float3& values);
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& os, const triangle& obj);
}
