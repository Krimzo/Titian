#pragma once

#include "render/render_pass.h"
#include "render/render_layer.h"


namespace titian {
    struct SkyboxPass : RenderPass
    {
        SkyboxPass();

        void state_package(StatePackage* package) override;
        void render_self(StatePackage* package) override;
    };
}
