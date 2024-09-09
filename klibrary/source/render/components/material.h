#pragma once

#include "render/components/texture.h"
#include "memory/memory.h"


namespace kl {
    struct Material
    {
        float texture_blend = 0.0f;
        float reflection_factor = 0.0f;
        float refraction_factor = 0.0f;
        float refraction_index = 1.0f;

        Float4 color = colors::WHITE;

        Ref<Texture> color_map;
        Ref<Texture> normal_map;
        Ref<Texture> roughness_map;
    };
}
