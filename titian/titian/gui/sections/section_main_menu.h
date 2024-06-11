#pragma once

#include "gui/gui_section.h"
#include "render/render_layer.h"
#include "editor/editor_layer.h"
#include "serialization/serializer.h"


namespace titian {
	class GUISectionMainMenu : public GUISection
	{
    public:
        GUISectionMainMenu(const LayerPackage& package);

        void render_gui() override;

    private:
        const std::string m_temp_path = "temp.titian";

        kl::Object<Texture> m_start_button_texture = nullptr;
        kl::Object<Texture> m_pause_button_texture = nullptr;
        kl::Object<Texture> m_stop_button_texture = nullptr;

        bool m_inputting_name = false;
        bool m_testing_exit = false;

        float m_control_buttons_width = 0.0f;
        float m_gizmo_buttons_width = 0.0f;
	};
}
