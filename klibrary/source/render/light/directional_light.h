#pragma once

#include "render/components/texture.h"
#include "render/scene/camera.h"
#include "graphics/graphics.h"
#include "memory/memory.h"


namespace kl {
    struct DirectionalLight : NoCopy
    {
        static constexpr int CASCADE_COUNT = 4;

        float cascades[CASCADE_COUNT + 1] = { 0.0f, 0.075f, 0.2f, 0.5f, 1.0f };
        float point_size = 1.0f;

        DirectionalLight(GPU& gpu, int map_resolution);

        void set_resolution(int resolution);
        int resolution() const;

        void set_direction(const Float3& direction);
        Float3 direction() const;

        dx::DepthView depth_view(UINT cascade_index) const;
        dx::ShaderView shader_view(UINT cascade_index) const;

        Float4x4 matrix(const Float4x4& inv_cam_mat) const;
        Float4x4 matrix(Camera camera, UINT cascade_index) const;

    private:
		GPU& m_gpu;
        int m_resolution;
        Float3 m_direction = { 0.0f, 0.0f, 1.0f };
        Ref<Texture> m_cascades[CASCADE_COUNT] = {};
    };
}
