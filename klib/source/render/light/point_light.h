#pragma once

#include "math/math.h"


namespace kl {
    class point_light
    {
    public:
        float3 origin = {};
        float3 color = colors::white;
        float intensity = 1.0f;

        point_light();
        ~point_light();
    };
}
