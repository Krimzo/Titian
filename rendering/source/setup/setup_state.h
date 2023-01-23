#pragma once

#include "state_machine/state_machine.h"


inline const std::string resouce_folder = "../resource/";

void setup_shaders(state_machine* state);
void setup_meshes(state_machine* state);
void setup_textures(state_machine* state);
void setup_scene(state_machine* state);
