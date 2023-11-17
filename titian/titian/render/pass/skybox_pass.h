#pragma once

#include "render/render_pass.h"
#include "render/render_layer.h"


namespace titian {
    class SkyboxPass : public RenderPass
    {
    public:
        RenderLayer* render_layer = nullptr;

        SkyboxPass(GameLayer* game_layer, RenderLayer* render_layer);

        bool is_renderable() const override;
        StatePackage get_state_package() override;
        void render_self(StatePackage& package) override;
    };
}