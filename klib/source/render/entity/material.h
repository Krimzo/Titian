#pragma once

#include "render/entity/texture.h"


namespace kl {
    class material
    {
    public:
        float4 color = {};

        ref<texture> color_map = nullptr;
        ref<texture> normal_map = nullptr;
        ref<texture> roughness_map = nullptr;

        float texture_blend = 0.0f;
        float reflection_factor = 0.0f;
        float refraction_factor = 0.0f;
        float refraction_index = 0.0f;

        material();
        virtual ~material();
    };
}
