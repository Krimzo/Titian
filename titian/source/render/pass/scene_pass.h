#pragma once

#include "render/render_pass.h"
#include "render/render_layer.h"
#include "editor/editor_layer.h"

#include "light/ambient_light.h"
#include "light/directional_light.h"


namespace titian {
    class ScenePass : public RenderPass
    {
    public:
        ScenePass();

        bool is_renderable() const override;
        StatePackage get_state_package() override;
        void render_self(StatePackage& package) override;
    };
}
