#pragma once

#include "render/render_pass.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    class EditorPass : public RenderPass
    {
    public:
        dx::Buffer frustum_mesh = nullptr;

        EditorPass();

        bool is_renderable() const override;
        StatePackage get_state_package() override;
        void render_self(StatePackage& package) override;
    };
}
