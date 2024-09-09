#pragma once

#include "render/render_pass.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    struct OutlinePass : RenderPass
    {
        OutlinePass();

        void state_package(StatePackage* package) override;
        void render_self(StatePackage* package) override;

    private:
        dx::Buffer m_selected_entities_buff;
        dx::ShaderView m_selected_entities_view;

        void load_selected_entities(const Vector<float>& entitiy_indices);
    };
}
