#pragma once

#include "math/vector/vector3.h"


namespace kl {
    struct vertex
    {
        float3 world = {};
        float2 texture = {};
        float3 normal = {};

        vertex()
        {}

        vertex(const float3& world)
            : world(world)
        {}

        vertex(const float3& world, const float2& texture)
            : world(world), texture(texture)
        {}

        vertex(const float3& world, const float3& normal)
            : world(world), normal(normal)
        {}

        vertex(const float3& world, const float2& texture, const float3& normal)
            : world(world), texture(texture), normal(normal)
        {}

        friend std::ostream& operator<<(std::ostream& stream, const vertex& obj)
        {
            stream << "{" << obj.world << ", " << obj.texture << ", " << obj.normal << "}";
            return stream;
        }
    };
}
