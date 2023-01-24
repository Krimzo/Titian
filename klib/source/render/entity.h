#pragma once

#include "render/mesh.h"
#include "render/material.h"
#include "math/matrix/matrix4x4.h"


namespace kl {
    class entity
    {
    public:
        float3 scale = float3::splash(1);
        float3 position = {};
        float3 rotation = {};

        float3 acceleration = {};
        float3 velocity = {};
        float3 angular = {};

        mesh mesh = {};
        material material = {};

        void* user_data = nullptr;

        entity();

        void update_physics(float delta_time);

        [[nodiscard]] mat4 matrix() const;
    };
}
