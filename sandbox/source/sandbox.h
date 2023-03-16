#pragma once

#include "editor.h"


int sandbox_main();

void setup_preview_scene(editor_state* state);

void   load_meshes(editor_state* state);
void load_textures(editor_state* state);
void load_skyboxes(editor_state* state);

void        setup_preview_cubes(editor_state* state, int size);
void      setup_preview_spheres(editor_state* state, int size);
void       setup_preview_monkes(editor_state* state, int size);
void      setup_preview_bullets(editor_state* state, int size);
void setup_preview_physics_test(editor_state* state, int size);
