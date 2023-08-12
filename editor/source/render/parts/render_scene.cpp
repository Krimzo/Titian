#include "editor.h"


void render_scene(editor_state* state)
{
    state->gpu->bind_raster_state(state->render_wireframe ? state->raster_states.wireframe : state->raster_states.solid);
    state->gpu->bind_depth_state(state->depth_states.enabled);
    state->gpu->bind_render_shaders(state->render_shaders.object_full);

    state->gpu->bind_sampler_state_for_pixel_shader(state->sampler_states.linear, 0);
    state->gpu->bind_sampler_state_for_pixel_shader(state->sampler_states.shadow, 1);
    state->gpu->bind_sampler_state_for_pixel_shader(state->sampler_states.linear, 2);

    if (kl::object<kl::texture>& skybox = state->scene->camera->skybox) {
        state->gpu->bind_shader_view_for_pixel_shader(skybox->shader_view, 0);
    }
    else {
        state->gpu->unbind_shader_view_for_pixel_shader(0);
    }

    struct VS_DATA
    {
        kl::float4x4  w_matrix;
        kl::float4x4 vp_matrix;

        kl::float4x4 vp_light_matrices[kl::directional_light::CASCADE_COUNT];
    } vs_data = {};

    vs_data.vp_matrix = state->scene->camera->matrix();

    if (state->scene->directional_light) {
        ID3D11ShaderResourceView* dir_light_views[kl::directional_light::CASCADE_COUNT] = {};
        for (int i = 0; i < kl::directional_light::CASCADE_COUNT; i++) {
            dir_light_views[i] = state->scene->directional_light->shader_view(i).Get();
            vs_data.vp_light_matrices[i] = state->scene->directional_light->matrix(*state->scene->camera, i);
        }
        state->gpu->context()->PSSetShaderResources(1, kl::directional_light::CASCADE_COUNT, dir_light_views);
    }

    struct PS_DATA
    {
        kl::float4 object_color; // (color.r, color.g, color.b, none)
        kl::float4 object_index; // (index, index, index, index)

        kl::float4     object_material; // (texture_blend, reflection_factor, refraction_factor, refraction_index)
        kl::float4 object_texture_info; // (has_normal_map, has_roughness_map, none, none)

        kl::float4 camera_info; // (camera.x, camera.y, camera.z, skybox?)
        kl::float4 camera_background; // (color.r, color.g, color.b, color.a)
        kl::float4x4  v_matrix; // View matrix

        kl::float4     ambient_light; // (color.r, color.g, color.b, intensity)
        kl::float4 directional_light; // (sun.x, sun.y, sun.z, sun_point_size)

        kl::float4   shadow_map_info; // (width, height, texel_width, texel_size)
        kl::float4 cascade_distances; // (cascade_0_far, cascade_1_far, cascade_2_far, cascade_3_far)
    } ps_data = {};

    ps_data.camera_info = { state->scene->camera->origin, (float) (bool) state->scene->camera->skybox };
    ps_data.camera_background = (kl::float4) state->scene->camera->background;
    ps_data.v_matrix = state->scene->camera->view_matrix();

    if (state->scene->ambient_light) {
        ps_data.ambient_light = { state->scene->ambient_light->color, state->scene->ambient_light->intensity };
    }

    if (state->scene->directional_light) {
        ps_data.directional_light = { state->scene->directional_light->direction(), state->scene->directional_light->point_size };

        ps_data.shadow_map_info = { kl::float2((float) state->scene->directional_light->map_resolution), kl::float2(1.0f / state->scene->directional_light->map_resolution) };
        for (int i = 0; i < kl::directional_light::CASCADE_COUNT; i++) {
            ps_data.cascade_distances[i] = kl::unwrap(state->scene->directional_light->CASCADE_SPLITS[i + 1], state->scene->camera->near_plane, state->scene->camera->far_plane);
        }
    }

    for (auto& [name, entity] : *state->scene) {
        if (!entity->mesh || !entity->material) { continue; }

        if (entity->material->color_map) {
            state->gpu->bind_shader_view_for_pixel_shader(entity->material->color_map->shader_view, 5);
        }

        if (entity->material->normal_map) {
            state->gpu->bind_shader_view_for_pixel_shader(entity->material->normal_map->shader_view, 6);
            ps_data.object_texture_info.x = 1.0f;
        }
        else {
            ps_data.object_texture_info.x = 0.0f;
        }

        if (entity->material->roughness_map) {
            state->gpu->bind_shader_view_for_pixel_shader(entity->material->roughness_map->shader_view, 7);
            ps_data.object_texture_info.y = 1.0f;
        }
        else {
            ps_data.object_texture_info.y = 0.0f;
        }

        vs_data.w_matrix = entity->matrix();

        ps_data.object_color = entity->material->color;
        ps_data.object_index = kl::float4 { (float) entity->unique_index };
        ps_data.object_material = {
            entity->material->texture_blend,
            entity->material->reflection_factor,
            entity->material->refraction_factor,
            entity->material->refraction_index,
        };

        state->render_shaders.object_full.vertex_shader.update_cbuffer(vs_data);
        state->render_shaders.object_full.pixel_shader.update_cbuffer(ps_data);

        state->gpu->draw(entity->mesh->graphics_buffer);
    }
}
