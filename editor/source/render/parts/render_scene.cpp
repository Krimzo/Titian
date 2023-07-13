#include "render/render_chain.h"

#include "cbuffers/entity_render.h"


void render_scene(editor_state* state)
{
    state->gpu->bind_raster_state(state->render_wireframe ? state->raster_states.wireframe : state->raster_states.solid);
    state->gpu->bind_depth_state(state->depth_states.enabled);
    state->gpu->bind_render_shaders(state->render_shaders.entity);

    state->gpu->bind_sampler_state_for_pixel_shader(state->sampler_states.linear, 0);
    state->gpu->bind_sampler_state_for_pixel_shader(state->sampler_states.shadow, 1);
    state->gpu->bind_sampler_state_for_pixel_shader(state->sampler_states.linear, 2);

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
    ps_cb.camera_info = { state->scene->camera->origin, 0 };
    ps_cb.v_matrix = state->scene->camera->view_matrix();

    if (state->scene->ambient_light) {
        ps_cb.ambient_light = { state->scene->ambient_light->color, state->scene->ambient_light->intensity };
    }

    if (state->scene->directional_light) {
        ps_cb.directional_light = { state->scene->directional_light->get_direction(), state->scene->directional_light->point_size };

        ps_cb.shadow_map_info = { kl::float2((float) state->scene->directional_light->map_resolution), kl::float2(1.0f / state->scene->directional_light->map_resolution) };
        for (int i = 0; i < kl::directional_light::CASCADE_COUNT; i++) {
            ps_cb.cascade_distances[i] = kl::interpolate(state->scene->directional_light->CASCADE_SPLITS[i + 1], state->scene->camera->near_plane, state->scene->camera->far_plane);
        }
    }

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

        state->render_shaders.entity.vertex_shader.update_cbuffer(vs_cb);
        state->render_shaders.entity.pixel_shader.update_cbuffer(ps_cb);

        state->gpu->draw(entity->mesh->graphics_buffer);
    }
}
