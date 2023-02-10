#include "render/render_chain.h"

#include "gui/gui_render.h"


void render_chain(state_machine* state)
{
    const kl::color background = state->scene ? state->scene->camera.background : kl::color();

    state->gpu->clear_internal_color((kl::float4) background);
    state->gpu->clear_internal_depth(1.0f);

    if (state->scene) {
        // Pre-render
        if (state->scene->directional_light) {
            state->gpu->set_viewport(kl::int2::splash(state->scene->directional_light->get_map_resolution()));
            state->gpu->unbind_all_targets();

            render_shadows(state);
        }

        // Scene render
        state->gpu->set_viewport(state->render_state->target_size);
        state->gpu->bind_targets({ state->render_state->render_target_view, state->render_state->picking_target_view });

        state->render_state->clear_targets((kl::float4) background);
        state->scene->camera.update_aspect_ratio(state->render_state->target_size);

        render_skybox(state);
        render_ocean(state);
        render_scene(state);

        // Postprocess render
        state->gpu->set_viewport(state->render_state->target_size);
        state->gpu->bind_targets({ state->render_state->render_target_view });

        render_postprocess(state);

        // GUI render
        state->gpu->set_viewport(state->window->size());
        state->gpu->bind_internal_targets();

        gui_render(state);
    }

    state->gpu->swap_buffers(v_sync);
}
