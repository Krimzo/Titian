#pragma once

#include "math/vector/float4.h"


namespace kl {
    struct vertex
    {
        float3 world = {};
        float2 texture = {};
        float3 normal = {};

        // Construct
        vertex();
        vertex(const float3& world);
        vertex(const float3& world, const float2& texture);
        vertex(const float3& world, const float3& normal);
        vertex(const float3& world, const float2& texture, const float3& normal);
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const vertex& obj);
}
