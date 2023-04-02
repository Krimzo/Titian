#pragma once

#include "render/components/texture.h"
#include "render/scene/camera.h"
#include "graphics/graphics.h"
#include "memory/memory.h"


namespace kl {
    class directional_light
    {
    public:
        static constexpr UINT CASCADE_COUNT = 4;
        static constexpr float CASCADE_SPLITS[CASCADE_COUNT + 1] = { 0.0f, 0.075f, 0.2f, 0.5f, 1.0f };

    private:
        float3 direction_ = { 0.0f, 0.0f, 1.0f };
        ref<texture> cascades_[CASCADE_COUNT] = {};

    public:
        const UINT map_resolution;
        float point_size = 1.0f;

        directional_light(gpu* gpu, UINT map_resolution);
        ~directional_light();

        directional_light(const directional_light&) = delete;
        directional_light(const directional_light&&) = delete;

        void operator=(const directional_light&) = delete;
        void operator=(const directional_light&&) = delete;

        void set_direction(const float3& direction);
        float3 get_direction() const;

        dx::depth_view get_depth_view(UINT cascade_index) const;
        dx::shader_view get_shader_view(UINT cascade_index) const;

        float4x4 get_matrix(camera camera, UINT cascade_index) const;
    };
}
