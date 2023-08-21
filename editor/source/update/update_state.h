#pragma once

#include "state/editor_state.h"


// Pre-Render
void update_physics(editor_state* state);
void update_scripts(editor_state* state);
void  handle_camera(editor_state* state);

// Post-Render
void finalize_frame(editor_state* state);
