export module render_chain;

export import editor_update;

export void render_shadows(EditorState* state);
export void render_skybox(EditorState* state);
export void render_scene(EditorState* state);
export void render_collider(EditorState* state);
export void render_postprocess(EditorState* state);

export void gui_render(EditorState* state);

export void render_chain(EditorState* state)
{
    // Clear
    const kl::Color background = (state->scene && state->scene->camera) ? state->scene->camera->background : kl::Color();
    state->gpu->clear_internal(background);

    // Scene
    if (state->scene && state->scene->camera) {
        // Pre-render
        if (state->scene->directional_light) {
            state->gpu->set_viewport_size(kl::Int2 { (int) state->scene->directional_light->map_resolution });
            state->gpu->unbind_target_depth_views();
            render_shadows(state);
        }

        // Scene render
        state->gpu->set_viewport_size(state->render_state->target_size);
        state->gpu->bind_target_depth_views({ state->render_state->render_target_view, state->render_state->picking_target_view }, state->gpu->internal_depth());
        state->scene->camera->update_aspect_ratio(state->render_state->target_size);
        state->render_state->clear_targets(state->gpu, background);
        if (state->scene->camera->skybox) {
            render_skybox(state);
        }
        render_scene(state);

        // Selected entity
        if (state->scene->selected_entity) {
            // Collider
            if (state->gui_state->render_collider && state->scene->selected_entity->collider()) {
                render_collider(state);
            }

            // Postprocess
            state->gpu->set_viewport_size(state->render_state->target_size);
            state->gpu->bind_target_depth_views({ state->render_state->render_target_view }, nullptr);
            render_postprocess(state);
        }
    }

    // GUI render
    state->gpu->set_viewport_size(state->window->size());
    state->gpu->bind_internal_views();
    gui_render(state);

    // Swap
    state->gpu->swap_buffers(state->v_sync);
}
