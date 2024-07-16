#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    class GUISectionAnimationEditor : public GUISection
    {
    public:
        kl::Object<Camera> camera = nullptr;
        kl::Object<Texture> render_texture = nullptr;
        kl::Object<Texture> depth_texture = nullptr;

        kl::Float4 line_color = kl::colors::WHITE;
        kl::Float3 sun_direction = { 0.0f, -1.0f, 0.0f };

        bool was_focused = true;
        int last_scroll = 0;
        kl::Float2 initial_camera_info = { 200.0f, 10.0f };
        kl::Float2 camera_info = initial_camera_info;

        std::string selected_animation = "/";

        GUISectionAnimationEditor(const LayerPackage& package);

        void render_gui() override;

    private:
        kl::Timer m_timer = {};
        bool m_animating = false;
        int m_frame_index = 0;
        int m_last_scroll = 0;
        int m_start_mesh_index = 0;

        void display_animations(kl::GPU* gpu, Scene* scene);
        void update_animation_camera();
        void render_selected_animation(kl::GPU* gpu, const Animation* animation, kl::Int2 viewport_size);
        void show_animation_properties(Animation* animation);
    };
}
