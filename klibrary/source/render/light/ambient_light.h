#pragma once

#include "math/math.h"


namespace kl {
    class AmbientLight
    {
    public:
        Float3 color = Float3(1.0f);
        float intensity = 0.1f;
    };
}
