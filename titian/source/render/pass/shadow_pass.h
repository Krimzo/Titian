#pragma once

#include "render/render_pass.h"
#include "editor/game_layer.h"
#include "render/render_layer.h"

#include "light/directional_light.h"


namespace titian {
    class ShadowPass : public RenderPass
    {
    public:
        ShadowPass(const LayerPackage& package);

        bool is_renderable() const override;
        StatePackage get_state_package() override;
        void render_self(StatePackage& package) override;
    };
}