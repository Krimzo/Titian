#pragma once

#include "state/editor_state.h"


// Consts
inline bool v_sync = true;

// Render
void handle_resizes(editor_state* state);
void   render_chain(editor_state* state);

// Render parts
void     render_shadows(editor_state* state);
void      render_skybox(editor_state* state);
void       render_scene(editor_state* state);
void    render_collider(editor_state* state);
void render_postprocess(editor_state* state);
