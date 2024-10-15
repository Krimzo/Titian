#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    struct GUISectionAnimationEditor : GUISection
    {
        Camera camera;
        Texture render_texture;
        Texture depth_texture;

        Float4 line_color = kl::colors::WHITE;
        Float3 sun_direction = { 0.0f, -1.0f, 0.0f };

        bool was_focused = true;
        Float2 initial_camera_info = { 200.0f, 10.0f };
        Float2 camera_info = initial_camera_info;

        String selected_animation = "/";

        GUISectionAnimationEditor();

        void render_gui() override;

    private:
        kl::Timer m_timer;
        bool m_animating = false;
        int m_frame_index = 0;
        int m_start_mesh_index = 0;

        void display_animations(Scene& scene);
        void update_animation_camera(Scene& scene);
        void render_selected_animation(Animation& animation, Int2 viewport_size);
        void show_animation_properties(Animation* animation);
    };
}
