#pragma once

#include "render/render_pass.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    class EditorPPPass : public RenderPass
    {
    public:
        EditorLayer* editor_layer = nullptr;
        GUILayer* gui_layer = nullptr;

        EditorPPPass(GameLayer* game_layer, EditorLayer* editor_layer, GUILayer* gui_layer);

        bool is_renderable() const override;
        StatePackage get_state_package() override;
        void render_self(StatePackage& package) override;
    };
}
