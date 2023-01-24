#pragma once

#include "math/vector/vector3.h"


namespace kl {
    struct sphere
    {
        float3 center = {};
        float radius = 0.0f;

        sphere()
        {}

        sphere(const float3& center, float radius) : center(center), radius(radius)
        {}

        friend std::ostream& operator<<(std::ostream& os, const sphere& obj)
        {
            os << "{" << obj.center << ", " << obj.radius << "}";
            return os;
        }
    };
}
