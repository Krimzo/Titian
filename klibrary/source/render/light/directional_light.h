#pragma once

#include "render/components/texture.h"
#include "render/scene/camera.h"
#include "graphics/graphics.h"
#include "memory/memory.h"


namespace kl {
    class DirectionalLight : NoCopy
    {
    public:
        static constexpr UINT CASCADE_COUNT = 4;
        static constexpr float CASCADE_SPLITS[CASCADE_COUNT + 1] = { 0.0f, 0.075f, 0.2f, 0.5f, 1.0f };

    private:
        Float3 m_direction = { 0.0f, 0.0f, 1.0f };
        Ref<Texture> m_cascades[CASCADE_COUNT] = {};

    public:
        const UINT map_resolution;
        float point_size = 1.0f;

        DirectionalLight(GPU* gpu, UINT map_resolution);

        void set_direction(const Float3& direction);
        Float3 direction() const;

        dx::DepthView depth_view(UINT cascade_index) const;
        dx::ShaderView shader_view(UINT cascade_index) const;

        Float4x4 matrix(Camera camera, UINT cascade_index) const;
    };
}
