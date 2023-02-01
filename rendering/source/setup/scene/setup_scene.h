#pragma once

#include "state/state_machine.h"


void setup_ocean(state_machine* state);

void setup_cubes(state_machine* state, int size);
void setup_spheres(state_machine* state, int size);

void setup_monkes(state_machine* state, int size);
void setup_bullets(state_machine* state, int size);

void setup_physics_test(state_machine* state, int size);
