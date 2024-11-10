#pragma once

#include "gui/gui_section.h"
#include "editor/game_layer.h"


namespace titian
{
struct GUISectionScriptingParameters : GUISection
{
    GUISectionScriptingParameters();

    void render_gui() override;

private:
    void display_interp_parameter_editor( int script_id, StringRef const& name, InterpScript::Parameter& parameter );
    void display_node_parameter_editor( int script_id, StringRef const& name, std::type_info const& type, void* ptr );
};
}
