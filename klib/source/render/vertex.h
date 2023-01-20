#pragma once

#include "math/math.h"


namespace kl {
    struct vertex
    {
        float3 world = {};
        float2 texture = {};
        float3 normal = {};

        vertex();
        explicit vertex(const float3& world);
        vertex(const float3& world, const float2& texture);
        vertex(const float3& world, const float3& normal);
        vertex(const float3& world, const float2& texture, const float3& normal);
    };

    std::ostream& operator<<(std::ostream& stream, const vertex& obj);
}
