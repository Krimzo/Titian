#pragma once

#include "state/state_machine.h"


// Consts
inline bool v_sync = true;
inline bool debug_display_depth = false;

// Render
void handle_resizes(state_machine* state);
void   render_chain(state_machine* state);

// Render parts
void  render_skybox(state_machine* state);
void   render_ocean(state_machine* state);

void render_shadows(state_machine* state);
void   render_scene(state_machine* state);
void  display_depth(state_machine* state);
