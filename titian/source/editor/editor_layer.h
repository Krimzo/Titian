#pragma once

#include "editor/game_layer.h"
#include "scene/camera.h"


namespace titian {
    class EditorLayer : public Layer
    {
    public:
        bool is_viewport_focused = false;
        bool is_over_viewport = false;

        int32_t gizmo_mode = 1;
        int32_t gizmo_operation = 0;
        int32_t outline_size = 1;

        Set<String> selected_entities;

        EditorLayer();

        void init();
        bool update() override;
    };
}