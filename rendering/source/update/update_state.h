#pragma once

#include "state/state_machine.h"


// Pre-Render
void update_physics(state_machine* state);
void   update_light(state_machine* state);
void  handle_camera(state_machine* state);

// Post-Render
void finalize_frame(state_machine* state);

// Custom
void custom_pre_render_update(state_machine* state);
void custom_post_render_update(state_machine* state);
