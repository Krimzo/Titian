#pragma once

#include "state/state_machine.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


// GUI
void gui_render(state_machine* state);

// GUI parts
void gui_scene_info(state_machine* state);
