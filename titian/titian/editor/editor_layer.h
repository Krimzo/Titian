#pragma once

#include "editor/game_layer.h"
#include "scene/camera.h"


namespace titian {
    class EditorLayer : public Layer
    {
    public:
        GameLayer* game_layer = nullptr;
        
        bool is_viewport_focused = false;
        bool is_over_viewport = false;

        int gizmo_mode = 1;
        int gizmo_operation = 0;

        std::string selected_entity = "/";

        EditorLayer(GameLayer* game_layer);

        bool update() override;
    };
}
