#pragma once

#include "gui/helper/gui_helper.h"
#include "state/editor_state.h"


// GUI
void gui_render(editor_state* state);

/* GUI parts */
// Top
void gui_main_menu(editor_state* state);
void gui_control_menu(editor_state* state);

// Left
void  gui_scene_items(editor_state* state);
void gui_script_items(editor_state* state);
void   gui_scene_info(editor_state* state);

// Middle-Top
void   gui_depth_display(editor_state* state);
void     gui_mesh_editor(editor_state* state);
void gui_material_editor(editor_state* state);
void    gui_scene_editor(editor_state* state);

// Middle-Bottom
void gui_log_view(editor_state* state);
void gui_explorer(editor_state* state);

// Right
void gui_entity_properties(editor_state* state);
void       gui_render_info(editor_state* state);
