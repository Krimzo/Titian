#pragma once

#include "gui/gui_section.h"
#include "render/render_layer.h"
#include "editor/editor_layer.h"
#include "serialization/binary_serializer.h"
#include "serialization/text_serializer.h"


namespace titian {
	class GUISectionMainMenu : public GUISection
	{
    public:
        GUISectionMainMenu();

        void render_gui() override;

    private:
        const String m_temp_path = "temp.titian";

        Ref<Texture> m_start_button_texture = nullptr;
        Ref<Texture> m_pause_button_texture = nullptr;
        Ref<Texture> m_stop_button_texture = nullptr;

        bool m_testing_exit = false;

        float m_control_buttons_width = 0.0f;
        float m_gizmo_buttons_width = 0.0f;
	};
}
