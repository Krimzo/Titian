#pragma once

#include "render/scene/camera.h"
#include "render/scene/entity.h"
#include "render/light/ambient_light.h"
#include "render/light/directional_light.h"


namespace kl {
    struct Scene
    {
        std::vector<Ref<Entity>> entities;

        Ref<Camera> main_camera = nullptr;
        Ref<AmbientLight> main_ambient_light = nullptr;
        Ref<DirectionalLight> main_directional_light = nullptr;

        Float3 gravity = { 0.0f, -9.81f, 0.0f };

        void update_physics(float delta_t);
    };
}
