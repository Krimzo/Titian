#pragma once

#include "render/render_pass.h"
#include "render/render_layer.h"


namespace titian {
    struct PostPass : RenderPass
    {
        PostPass();

        void state_package(StatePackage& package) override;
        void render_self(StatePackage& package) override;
    };
}
