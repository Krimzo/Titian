#pragma once

#include "render/render_pass.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    class OutlinePass : public RenderPass
    {
    public:
        OutlinePass();

        bool is_renderable() const override;
        StatePackage get_state_package() override;
        void render_self(StatePackage& package) override;

    private:
        dx::Buffer m_selected_entities_buff;
        dx::ShaderView m_selected_entities_view;

        void load_selected_entities(const Vector<float>& entitiy_indices);
    };
}
