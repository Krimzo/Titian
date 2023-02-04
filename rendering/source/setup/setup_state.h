#pragma once

#include "state/state_machine.h"


void   setup_depth_states(state_machine* state);
void setup_sampler_states(state_machine* state);
void  setup_raster_states(state_machine* state);

void  setup_shaders(state_machine* state);
void   setup_meshes(state_machine* state);
void setup_textures(state_machine* state);
void setup_skyboxes(state_machine* state);

void setup_scene(state_machine* state);

void setup_gui(state_machine* state);
