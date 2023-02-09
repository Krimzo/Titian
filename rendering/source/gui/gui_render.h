#pragma once

#include "state/state_machine.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


// GUI
void gui_render(state_machine* state);

/* GUI parts */
// Left
void       gui_scene_items(state_machine* state);
void        gui_scene_info(state_machine* state);

// Middle
void      gui_viewport(state_machine* state);
void gui_depth_display(state_machine* state);

// Right
void gui_entity_properties(state_machine* state);
