#include "render/render_chain.h"

#include "cbuffers/shadow_render.h"


void render_shadows(state_machine* state)
{
    state->gpu->bind_raster_state(state->raster_states["shadow"]);
    state->gpu->bind_depth_state(state->depth_states["shadow"]);

    state->gpu->bind_shaders(state->shaders["shadow"]);

    for (int i = 0; i < kl::directional_light::CASCADE_COUNT; i++) {
        const kl::mat4 VP = state->scene->directional_light->get_matrix(state->scene->camera, i);
        const kl::dx::depth_view shadow_map = state->scene->directional_light->get_depth_view(i);

        state->gpu->bind_targets({}, shadow_map);
        state->gpu->clear_depth_view(shadow_map);

        shadow_render_vs_cb vs_cb = {};

        for (auto& [name, entity] : *state->scene) {
            if (!entity->mesh) { continue; }

            vs_cb.WVP = VP * entity->matrix();
            state->gpu->set_vertex_const_buffer(vs_cb);

            state->gpu->draw_vertex_buffer(entity->mesh->graphics_buffer);
        }
    }
}
