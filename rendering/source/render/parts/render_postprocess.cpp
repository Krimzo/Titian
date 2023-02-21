#include "render/render_chain.h"

#include "cbuffers/postprocess_render.h"


void render_postprocess(state_machine* state)
{
    state->gpu->bind_raster_state(state->raster_states["postprocess"]);
    state->gpu->bind_depth_state(state->depth_states["postprocess"]);

    state->gpu->bind_render_shaders(state->shaders["postprocess"]);

    state->gpu->bind_shader_view_for_pixel_shader(state->render_state->picking_shader_view, 0);

    postprocess_ps_cb ps_cb = {};
    ps_cb.selected_index = kl::float4((float) state->scene->selected_entity->unique_index);
    ps_cb.outline_color = kl::colors::orange;

    state->gpu->bind_cb_for_pixel_shader(postprocess_ps_const_buffer, 0);
    state->gpu->set_cb_data(postprocess_ps_const_buffer, ps_cb);

    state->gpu->draw_mesh(state->render_state->screen_mesh);
}
