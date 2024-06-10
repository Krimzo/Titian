#pragma once

#include "render/render_pass.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    class OutlinePass : public RenderPass
    {
    public:
        EditorLayer* editor_layer = nullptr;
        GUILayer* gui_layer = nullptr;

        OutlinePass(GameLayer* game_layer, EditorLayer* editor_layer, GUILayer* gui_layer);

        bool is_renderable() const override;
        StatePackage get_state_package() override;
        void render_self(StatePackage& package) override;

    private:
        kl::dx::Buffer m_selected_entities_buff;
        kl::dx::ShaderView m_selected_entities_view;

        void load_selected_entities(const std::vector<float>& entitiy_indices);
    };
}
