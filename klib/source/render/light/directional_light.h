#pragma once

#include "render/camera.h"
#include "graphics/gpu.h"


namespace kl {
    class directional_light
    {
    public:
        static constexpr int MAP_COUNT = 4;
        static constexpr float CASCADE_SPLITS[MAP_COUNT + 1] = { 0.0f, 0.075f, 0.2f, 0.5f, 1.0f };

    private:
        float3 direction_ = { 0, 0, 1 };
        int map_resolution_ = 0;

        gpu* gpu_ = nullptr;
        dx::depth_view   shadow_depth_views_[MAP_COUNT] = {};
        dx::shader_view shadow_shader_views_[MAP_COUNT] = {};

    public:
        float point_size = 1.0f;

        directional_light(gpu* gpu, int map_resolution);
        ~directional_light();

        directional_light(const directional_light&) = delete;
        directional_light(const directional_light&&) = delete;

        void operator=(const directional_light&) = delete;
        void operator=(const directional_light&&) = delete;

        void set_direction(const float3& direction);
        float3 get_direction() const;

        int get_map_resolution() const;

        const dx::depth_view* get_depth_views() const;
        const dx::shader_view* get_shader_views() const;

        dx::depth_view   get_depth_view(int cascade_index) const;
        dx::shader_view get_shader_view(int cascade_index) const;

        mat4 get_matrix(camera camera, int cascade_index) const;
    };
}
