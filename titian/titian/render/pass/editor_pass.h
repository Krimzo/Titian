#pragma once

#include "render/render_pass.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    class EditorPass : public RenderPass
    {
    public:
        kl::dx::Buffer frustum_mesh = nullptr;

        EditorPass(const LayerPackage& package);

        bool is_renderable() const override;
        StatePackage get_state_package() override;
        void render_self(StatePackage& package) override;
    };
}
