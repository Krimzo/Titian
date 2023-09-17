export module render_postprocess;

export import render_chain;

export void render_postprocess(editor_state* state)
{
    state->gpu->bind_raster_state(state->raster_states.solid);
    state->gpu->bind_depth_state(state->depth_states.disabled);
    state->gpu->bind_render_shaders(state->render_shaders.postprocess_pass);

    state->gpu->bind_shader_view_for_pixel_shader(state->render_state->picking_shader_view, 0);

    struct PS_DATA
    {
        kl::float4 selected_index;
        kl::float4  outline_color;
    } ps_data = {};

    ps_data.selected_index = kl::float4((float) state->scene->selected_entity->unique_index);
    ps_data.outline_color = state->gui_state->color_special;
    state->render_shaders.postprocess_pass.pixel_shader.update_cbuffer(ps_data);

    state->gpu->draw(state->render_state->screen_mesh);
}
