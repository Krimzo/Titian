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

        int32_t gizmo_mode = 1;
        int32_t gizmo_operation = 0;
        int32_t outline_size = 1;

        std::string selected_entity = "/";

        EditorLayer(GameLayer* game_layer);

        bool update() override;
    };
}
