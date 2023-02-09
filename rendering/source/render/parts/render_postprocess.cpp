#include "render/render_chain.h"

#include "cbuffers/postprocess_render.h"


void render_postprocess(state_machine* state)
{
    if (!state->scene->selected_entity) { return; }

    state->gpu->bind_raster_state(state->raster_states["postprocess"]);
    state->gpu->bind_depth_state(state->depth_states["postprocess"]);

    state->gpu->bind_shaders(state->shaders["postprocess"]);

    state->gpu->bind_pixel_shader_view(state->render_state->picking_shader_view, 0);

    postprocess_ps_cb ps_cb = {};
    ps_cb.selected_index = kl::float4::splash(state->scene->selected_entity->unique_index);
    ps_cb.outline_color = (kl::float4) kl::colors::orange;

    state->gpu->set_pixel_const_buffer(ps_cb);

    state->gpu->draw_vertex_buffer(state->render_state->screen_mesh);
}
