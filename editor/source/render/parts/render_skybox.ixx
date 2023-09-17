export module render_skybox;

export import render_chain;

export void render_skybox(editor_state* state)
{
    state->gpu->bind_raster_state(state->raster_states.solid);
    state->gpu->bind_depth_state(state->depth_states.disabled);
    state->gpu->bind_render_shaders(state->render_shaders.skybox_sample);

    state->gpu->bind_sampler_state_for_pixel_shader(state->sampler_states.linear, 0);
    state->gpu->bind_shader_view_for_pixel_shader(state->scene->camera->skybox->shader_view, 0);

    struct VS_DATA
    {
        kl::float4x4 vp_matrix;
    } vs_data = {};

    vs_data.vp_matrix = state->scene->camera->matrix();
    state->render_shaders.skybox_sample.vertex_shader.update_cbuffer(vs_data);

    state->gpu->draw(state->default_mesh.cube->graphics_buffer);
}
