#pragma once

#include "math/math.h"


namespace kl {
    class PointLight
    {
    public:
        Float3 origin{};
        Float3 color{ colors::WHITE };
        float intensity{ 1.0f };
    };
}
