#pragma once

#include "state_machine/state_machine.h"


void update_physics(state_machine* state);
void   update_light(state_machine* state);
void  handle_camera(state_machine* state);

void  custom_update(state_machine* state);

void handle_resizes(state_machine* state);
void   render_scene(state_machine* state);

void finalize_frame(state_machine* state);
