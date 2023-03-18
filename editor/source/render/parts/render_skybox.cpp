#include "render/render_chain.h"

#include "cbuffers/skybox_render.h"


void render_skybox(editor_state* state)
{
    state->gpu->bind_raster_state(state->raster_states["skybox"]);
    state->gpu->bind_depth_state(state->depth_states["skybox"]);

    state->gpu->bind_render_shaders(state->shaders["skybox"]);

    state->gpu->bind_sampler_state_for_pixel_shader(state->sampler_states["skybox"], 0);
    state->gpu->bind_shader_view_for_pixel_shader(state->scene->camera->skybox->shader_view, 0);

    skybox_render_vs_cb vs_cb = {};
    vs_cb.vp_matrix = state->scene->camera->matrix();

    state->gpu->bind_cb_for_vertex_shader(skybox_render_vs_const_buffer, 0);
    state->gpu->set_cb_data(skybox_render_vs_const_buffer, vs_cb);

    state->gpu->draw_mesh(state->default_meshes["cube"]->graphics_buffer);
}
