#pragma once

#include "render/components/mesh.h"
#include "render/components/material.h"


namespace kl {
    struct Entity
    {
        Float3 scale{ 1.0f };
        Float3 rotation;
        Float3 position;

        Float3 acceleration;
        Float3 velocity;
        Float3 angular;

        Ref<Mesh> mesh;
        Ref<Material> material;

        void update_physics(float delta_t);

        Float4x4 matrix() const;
    };
}
