#pragma once

#include "math/vector/float4.h"


namespace kl {
    class Sphere
    {
    public:
        Float3 origin = {};
        float radius = 0.0f;

        Sphere();
        Sphere(const Float3& origin, float radius);
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& os, const Sphere& obj);
}
