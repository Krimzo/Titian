#pragma once

#include "math/math.h"


namespace kl {
    struct PointLight
    {
        Float3 origin{};
        Float3 color{ colors::WHITE };
        float intensity{ 1.0f };
    };
}
