#pragma once

#include "render/components/texture.h"
#include "memory/memory.h"


namespace kl {
    class Material
    {
    public:
        float texture_blend = 0.0f;
        float reflection_factor = 0.0f;
        float refraction_factor = 0.0f;
        float refraction_index = 1.0f;

        Float4 color = colors::WHITE;

        Object<Texture> color_map = nullptr;
        Object<Texture> normal_map = nullptr;
        Object<Texture> roughness_map = nullptr;
    };
}
