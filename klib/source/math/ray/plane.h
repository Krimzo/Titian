#pragma once

#include "math/vector/float4.h"


namespace kl {
    struct plane
    {
    private:
        float3 normal_ = { 0.0f, 1.0f, 0.0f };

    public:
        float3 origin = {};

        plane();
        plane(const float3& origin, const float3& normal);

        // Normal
        void set_normal(const float3& normal);
        float3 get_normal() const;

        // Math
        bool is_in_front(const float3& point) const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& os, const plane& obj);
}
