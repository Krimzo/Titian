#pragma once

#include "math/vector/float4.h"


namespace kl {
    class Plane
    {
        Float3 m_normal = { 0.0f, 1.0f, 0.0f };

    public:
        Float3 origin = {};

        Plane();
        Plane(const Float3& origin, const Float3& normal);

        // Normal
        void set_normal(const Float3& normal);
        Float3 normal() const;

        // Math
        bool in_front(const Float3& point) const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& os, const Plane& obj);
}
