#include "setup/setup_state.h"

#include "cbuffers/shadow_render.h"
#include "cbuffers/skybox_render.h"
#include "cbuffers/entity_render.h"
#include "cbuffers/collider_render.h"
#include "cbuffers/postprocess_render.h"


void setup_cbuffers(editor_state* state)
{
    shadow_render_vs_const_buffer = state->gpu->create_const_buffer(sizeof(shadow_render_vs_cb));

    skybox_render_vs_const_buffer = state->gpu->create_const_buffer(sizeof(skybox_render_vs_cb));

    entity_render_vs_const_buffer = state->gpu->create_const_buffer(sizeof(entity_render_vs_cb));
    entity_render_ps_const_buffer = state->gpu->create_const_buffer(sizeof(entity_render_ps_cb));

    collider_render_vs_const_buffer = state->gpu->create_const_buffer(sizeof(collider_render_vs_cb));
    collider_render_ps_const_buffer = state->gpu->create_const_buffer(sizeof(collider_render_ps_cb));

    postprocess_ps_const_buffer = state->gpu->create_const_buffer(sizeof(postprocess_ps_cb));
}
