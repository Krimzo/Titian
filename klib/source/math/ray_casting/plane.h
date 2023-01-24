#pragma once

#include "math/vector/vector3.h"


namespace kl {
    struct plane
    {
        float3 normal = {};
        float3 point = {};

        plane()
        {}

        plane(const float3& normal, const float3& point)
            : normal(normal.normalize()), point(point)
        {}

        friend std::ostream& operator<<(std::ostream& os, const plane& obj)
        {
            os << "{" << obj.normal << ", " << obj.point << "}";
            return os;
        }
    };
}
