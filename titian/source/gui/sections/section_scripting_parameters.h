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
        void display_parameter_editor(int script_id, const String& name, const cs::Boxed_Value& parameter);
    };
}
