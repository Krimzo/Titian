#include "editor.h"


void render_shadows(editor_state* state)
{
    state->gpu->bind_raster_state(state->raster_states.shadow);
    state->gpu->bind_depth_state(state->depth_states.enabled);
    state->gpu->bind_render_shaders(state->render_shaders.shadow_pass);

    for (int i = 0; i < kl::directional_light::CASCADE_COUNT; i++) {
        const kl::float4x4 VP = state->scene->directional_light->matrix(*state->scene->camera, i);
        const kl::dx::depth_view shadow_map = state->scene->directional_light->depth_view(i);

        state->gpu->bind_target_depth_views({}, shadow_map);
        state->gpu->clear_depth_view(shadow_map);

        for (auto& [name, entity] : *state->scene) {
            if (!entity->mesh) { continue; }

            struct VS_DATA
            {
                kl::float4x4 WVP;
            } vs_data = {};

            vs_data.WVP = VP * entity->matrix();
            state->render_shaders.shadow_pass.vertex_shader.update_cbuffer(vs_data);

            state->gpu->draw(entity->mesh->graphics_buffer);
        }
    }
}
