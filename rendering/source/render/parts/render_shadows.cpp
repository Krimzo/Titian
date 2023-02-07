#include "render/render_chain.h"

#include "cbuffers/shadow_render.h"


void render_shadows(state_machine* state)
{
    const kl::int2 old_viewport = state->gpu->get_viewport();
    const int shadow_map_resolution = state->scene->directional_light->get_map_resolution();
    state->gpu->set_viewport({ shadow_map_resolution , shadow_map_resolution });

    state->gpu->bind_raster_state(state->raster_states["shadow"]);
    state->gpu->bind_depth_state(state->depth_states["shadow"]);

    state->gpu->bind_shaders(state->shaders["shadow"]);

    for (int i = 0; i < kl::directional_light::MAP_COUNT; i++) {
        const kl::mat4 VP = state->scene->directional_light->get_matrix(state->scene->camera, i);
        const kl::dx::depth_view shadow_map = state->scene->directional_light->get_depth_view(i);

        state->gpu->bind_targets({}, shadow_map);
        state->gpu->clear_depth_view(shadow_map);

        shadow_render_vs_cb vs_cb = {};

        for (auto& entity : *state->scene) {
            if (!entity->mesh) { continue; }

            vs_cb.WVP = VP * entity->matrix();
            state->gpu->set_vertex_const_buffer(vs_cb);

            state->gpu->draw_vertex_buffer(entity->mesh->graphics_buffer);
        }
    }

    state->gpu->bind_internal_targets();
    state->gpu->set_viewport(old_viewport);
}
