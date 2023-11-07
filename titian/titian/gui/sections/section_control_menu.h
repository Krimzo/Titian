#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"


namespace titian {
    class GUISectionControlMenu : public GUISection
    {
    public:
        EditorLayer* editor_layer = nullptr;
        
        GUISectionControlMenu(EditorLayer* editor_layer);

        void render_gui() override;

    private:
        const std::string m_temp_path = "temp.titian";

        void start_scene();
        void stop_scene();
    };
}
