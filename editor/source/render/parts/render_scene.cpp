#include "render/render_chain.h"

#include "cbuffers/entity_render.h"


void render_scene(editor_state* state)
{
    state->gpu->bind_raster_state(state->raster_states["entity"]);
    state->gpu->bind_depth_state(state->depth_states["entity"]);

    state->gpu->bind_render_shaders(state->shaders["entity"]);

    state->gpu->bind_sampler_state_for_pixel_shader(state->sampler_states["skybox"], 0);
    state->gpu->bind_sampler_state_for_pixel_shader(state->sampler_states["shadow"], 1);
    state->gpu->bind_sampler_state_for_pixel_shader(state->sampler_states["entity"], 5);

    state->gpu->bind_shader_view_for_pixel_shader(state->scene->camera->skybox->shader_view, 0);

    entity_render_vs_cb vs_cb = {};
    vs_cb.vp_matrix = state->scene->camera->matrix();

    if (state->scene->directional_light) {
        ID3D11ShaderResourceView* dir_light_views[kl::directional_light::CASCADE_COUNT] = {};
        for (int i = 0; i < kl::directional_light::CASCADE_COUNT; i++) {
            dir_light_views[i] = state->scene->directional_light->get_shader_view(i).Get();
            vs_cb.vp_light_matrices[i] = state->scene->directional_light->get_matrix(*state->scene->camera, i);
        }
        state->gpu->context()->PSSetShaderResources(1, kl::directional_light::CASCADE_COUNT, dir_light_views);
    }

    entity_render_ps_cb ps_cb = {};
    ps_cb.camera_info = { state->scene->camera->position, 0 };
    ps_cb.v_matrix = state->scene->camera->view_matrix();

    if (state->scene->ambient_light) {
        ps_cb.ambient_light = { state->scene->ambient_light->color, state->scene->ambient_light->intensity };
    }

    if (state->scene->directional_light) {
        ps_cb.directional_light = { state->scene->directional_light->get_direction(), state->scene->directional_light->point_size };

        ps_cb.shadow_map_info = { kl::float2((float) state->scene->directional_light->get_map_resolution()), kl::float2(1.0f / state->scene->directional_light->get_map_resolution()) };
        for (int i = 0; i < kl::directional_light::CASCADE_COUNT; i++) {
            ps_cb.cascade_distances[i] = kl::math::interpolate(state->scene->directional_light->CASCADE_SPLITS[i + 1], state->scene->camera->near_plane, state->scene->camera->far_plane);
        }
    }

    state->gpu->bind_cb_for_vertex_shader(entity_render_vs_const_buffer, 0);
    state->gpu->bind_cb_for_pixel_shader(entity_render_ps_const_buffer, 0);

    for (auto& [name, entity] : *state->scene) {
        if (!entity->mesh || !entity->material) { continue; }

        if (entity->material->color_map) {
            state->gpu->bind_shader_view_for_pixel_shader(entity->material->color_map->shader_view, 5);
        }

        if (entity->material->normal_map) {
            state->gpu->bind_shader_view_for_pixel_shader(entity->material->normal_map->shader_view, 6);
            ps_cb.object_texture_info.x = 1.0f;
        }
        else {
            ps_cb.object_texture_info.x = 0.0f;
        }

        if (entity->material->roughness_map) {
            state->gpu->bind_shader_view_for_pixel_shader(entity->material->roughness_map->shader_view, 7);
            ps_cb.object_texture_info.y = 1.0f;
        }
        else {
            ps_cb.object_texture_info.y = 0.0f;
        }

        vs_cb.w_matrix = entity->matrix();

        ps_cb.object_color = entity->material->color;
        ps_cb.object_index = kl::float4((float) entity->unique_index);

        ps_cb.object_material.x = entity->material->texture_blend;
        ps_cb.object_material.y = entity->material->reflection_factor;
        ps_cb.object_material.z = entity->material->refraction_factor;
        ps_cb.object_material.w = entity->material->refraction_index;

        state->gpu->set_cb_data(entity_render_vs_const_buffer, vs_cb);
        state->gpu->set_cb_data(entity_render_ps_const_buffer, ps_cb);

        state->gpu->draw_mesh(entity->mesh->graphics_buffer);
    }
}
