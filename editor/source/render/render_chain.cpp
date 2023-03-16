#include "render/render_chain.h"

#include "gui/gui_render.h"


void render_chain(editor_state* state)
{
    // Clear
    const kl::color background = (state->scene && state->scene->camera) ? state->scene->camera->background : kl::color();
    state->gpu->clear_internal(background);

    // Scene
    if (state->scene && state->scene->camera) {
        // Pre-render
        if (state->scene->directional_light) {
            state->gpu->set_viewport_size(kl::int2(state->scene->directional_light->get_map_resolution()));
            state->gpu->unbind_target_depth_views();
            render_shadows(state);
        }

        // Scene render
        state->gpu->set_viewport_size(state->render_state->target_size);
        state->gpu->bind_target_depth_views({ state->render_state->render_target_view, state->render_state->picking_target_view }, state->gpu->get_internal_depth());
        state->render_state->clear_targets(background);
        state->scene->camera->update_aspect_ratio(state->render_state->target_size);

        if (state->scene->camera->skybox) {
            render_skybox(state);
        }
        render_scene(state);

        // Postprocess render
        if (state->scene->selected_entity) {
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
    state->gpu->swap_buffers(v_sync);
}
