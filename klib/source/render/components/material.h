#pragma once

#include "render/components/texture.h"
#include "memory/memory.h"


namespace kl {
    class material
    {
    public:
        float texture_blend = 0.0f;
        float reflection_factor = 0.0f;
        float refraction_factor = 0.0f;
        float refraction_index = 1.0f;

        float4 color = colors::white;

        object<texture> color_map = nullptr;
        object<texture> normal_map = nullptr;
        object<texture> roughness_map = nullptr;
    };
}
