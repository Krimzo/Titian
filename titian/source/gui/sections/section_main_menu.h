#pragma once

#include "gui/gui_section.h"
#include "render/render_layer.h"
#include "editor/editor_layer.h"
#include "serial/binary_serializer.h"
#include "serial/text_serializer.h"


namespace titian {
    struct GUISectionMainMenu : GUISection
	{
        GUISectionMainMenu();

        void render_gui() override;

    private:
        const String m_temp_path = "temp.titian";

        Texture m_start_button_texture;
        Texture m_pause_button_texture;
        Texture m_stop_button_texture;

        bool m_testing_exit = false;

        float m_control_buttons_width = 0.0f;
        float m_gizmo_buttons_width = 0.0f;
	};
}
