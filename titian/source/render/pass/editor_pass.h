#pragma once

#include "render/render_pass.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    struct EditorPass : RenderPass
    {
        dx::Buffer frustum_mesh;

        EditorPass();

        void state_package(StatePackage& package) override;
        void render_self(StatePackage& package) override;
    };
}
