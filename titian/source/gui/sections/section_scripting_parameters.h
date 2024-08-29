#pragma once

#include "gui/gui_section.h"
#include "editor/game_layer.h"


namespace titian {
    class GUISectionScriptingParameters : public GUISection
    {
    public:
        GUISectionScriptingParameters();

        void render_gui() override;

    private:
        void display_interp_parameter_editor(int script_id, const StringView& name, InterpScript::Parameter& parameter);
        void display_node_parameter_editor(int script_id, const StringView& name, const std::type_info& type, void* ptr);
    };
}
