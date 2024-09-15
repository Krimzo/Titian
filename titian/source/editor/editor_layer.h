#pragma once

#include "editor/game_layer.h"
#include "scene/camera.h"


namespace titian {
    struct EditorLayer : Layer
    {
        bool is_viewport_focused = false;
        bool is_over_viewport = false;

        ImGuizmo::OPERATION gizmo_operation = ImGuizmo::OPERATION::NONE;
        ImGuizmo::MODE gizmo_mode = ImGuizmo::MODE::WORLD;
        int outline_size = 1;

        StringSet selected_entities;

        EditorLayer();

        void init();
        bool update() override;
    };
}
