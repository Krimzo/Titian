#pragma once

#include "state/editor_state.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include <imgui.h>
#include <ImGuizmo.h>


// GUI
void gui_render(editor_state* state);

/* GUI parts */
// Left
void gui_scene_items(editor_state* state);
void  gui_scene_info(editor_state* state);

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