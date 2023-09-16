#pragma once

#include "state/editor_state.h"


void setup_depth_states(editor_state* state);
void setup_sampler_states(editor_state* state);
void setup_raster_states(editor_state* state);

void setup_shaders(editor_state* state);

void setup_gui(editor_state* state);
void load_custom_theme(editor_state* state);
void load_custom_font(const editor_state* state);
