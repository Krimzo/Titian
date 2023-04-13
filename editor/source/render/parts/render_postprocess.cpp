#include "render/render_chain.h"

#include "cbuffers/postprocess_render.h"


void render_postprocess(editor_state* state)
{
    state->gpu->bind_raster_state(state->raster_states.solid);
    state->gpu->bind_depth_state(state->depth_states.disabled);
    state->gpu->bind_render_shaders(state->render_shaders.postprocess);

    state->gpu->bind_shader_view_for_pixel_shader(state->render_state->picking_shader_view, 0);

    postprocess_ps_cb ps_cb = {};
    ps_cb.selected_index = kl::float4((float) state->scene->selected_entity->unique_index);
    ps_cb.outline_color = state->gui_state.selection_color;
    state->render_shaders.postprocess.pixel_shader.update_cbuffer(ps_cb);

    state->gpu->draw(state->render_state->screen_mesh);
}
