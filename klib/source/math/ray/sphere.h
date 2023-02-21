#pragma once

#include "math/vector/float4.h"


namespace kl {
    struct sphere
    {
        float3 origin = {};
        float radius = 0.0f;

        sphere();
        sphere(const float3& origin, float radius);
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& os, const sphere& obj);
}
