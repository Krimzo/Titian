#include "render/render_chain.h"

#include "cbuffers/shadow_render.h"


void render_shadows(editor_state* state)
{
    state->gpu->bind_raster_state(state->raster_states["shadow"]);
    state->gpu->bind_depth_state(state->depth_states["shadow"]);

    state->gpu->bind_render_shaders(state->shaders["shadow"]);

    for (int i = 0; i < kl::directional_light::CASCADE_COUNT; i++) {
        const kl::float4x4 VP = state->scene->directional_light->get_matrix(*state->scene->camera, i);
        const kl::dx::depth_view shadow_map = state->scene->directional_light->get_depth_view(i);

        state->gpu->bind_target_depth_views({}, shadow_map);
        state->gpu->clear_depth_view(shadow_map);

        shadow_render_vs_cb vs_cb = {};
        state->gpu->bind_cb_for_vertex_shader(shadow_render_vs_const_buffer, 0);

        for (auto& [name, entity] : *state->scene) {
            if (!entity->mesh) { continue; }

            vs_cb.WVP = VP * entity->matrix();
            state->gpu->set_cb_data(shadow_render_vs_const_buffer, vs_cb);

            state->gpu->draw_mesh(entity->mesh->graphics_buffer);
        }
    }
}
