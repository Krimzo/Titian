#pragma once

#include "math/math.h"


namespace kl {
    class ambient_light
    {
    public:
        float3 color = float3(1.0f);
        float intensity = 0.1f;

        ambient_light();
        ~ambient_light();
    };
}
