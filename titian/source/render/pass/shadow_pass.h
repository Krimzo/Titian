#pragma once

#include "render/render_pass.h"
#include "editor/game_layer.h"
#include "render/render_layer.h"
#include "scene/light/directional_light.h"


namespace titian {
    struct ShadowPass : RenderPass
    {
        ShadowPass();

        void state_package(StatePackage& package) override;
        void render_self(StatePackage& package) override;
    };
}
