#include "render/render_chain.h"

#include "cbuffers/entity_render.h"


void render_scene(state_machine* state)
{
	state->gpu->bind_raster_state(state->raster_states["entity"]);
	state->gpu->bind_depth_state(state->depth_states["entity"]);

	state->gpu->bind_shaders(state->shaders["entity"]);
	
	state->gpu->bind_sampler_state(state->sampler_states["skybox"], 0);
	state->gpu->bind_sampler_state(state->sampler_states["shadow"], 1);
	state->gpu->bind_sampler_state(state->sampler_states["entity"], 5);

	state->gpu->bind_pixel_shader_view(state->scene->camera.skybox, 0);
	state->gpu->context()->PSSetShaderResources(1, kl::directional_light::MAP_COUNT, state->scene->directional_light->get_shader_views());

	entity_render_vs_cb vs_cb = {};
	vs_cb.vp_matrix = state->scene->camera.matrix();
	for (int i = 0; i < kl::directional_light::MAP_COUNT; i++) {
		vs_cb.vp_light_matrices[i] = state->scene->directional_light->get_matrix(state->scene->camera, i);
	}

	entity_render_ps_cb ps_cb = {};
	ps_cb.camera_info = { state->scene->camera.position, 0 };
	ps_cb.v_matrix = state->scene->camera.view_matrix();

	ps_cb.ambient_light = { state->scene->ambient_light->color, state->scene->ambient_light->intensity };
	ps_cb.directional_light = { state->scene->directional_light->get_direction(), state->scene->directional_light->point_size };

	ps_cb.shadow_map_info = { kl::float2::splash(state->scene->directional_light->get_map_resolution()), kl::float2::splash(1.0f / state->scene->directional_light->get_map_resolution()) };
	for (int i = 0; i < kl::directional_light::MAP_COUNT; i++) {
		ps_cb.cascade_distances[i] = kl::math::interpolate(state->scene->directional_light->CASCADE_SPLITS[i + 1], state->scene->camera.near_plane, state->scene->camera.far_plane);
	}

	for (auto& entity : *state->scene) {
		if (!entity->mesh) { continue; }

		if (entity->material.color_map) {
			state->gpu->bind_pixel_shader_view(entity->material.color_map, 5);
		}

		if (entity->material.normal_map) {
			state->gpu->bind_pixel_shader_view(entity->material.normal_map, 6);
			ps_cb.object_texture_info.x = 1.0f;
		}
		else {
			ps_cb.object_texture_info.x = 0.0f;
		}

		if (entity->material.roughness_map) {
			state->gpu->bind_pixel_shader_view(entity->material.roughness_map, 7);
			ps_cb.object_texture_info.y = 1.0f;
		}
		else {
			ps_cb.object_texture_info.y = 0.0f;
		}

		vs_cb.w_matrix = entity->matrix();

		ps_cb.object_color  = entity->material.color;
		ps_cb.object_material.x = entity->material.texture_blend;
		ps_cb.object_material.y = entity->material.reflection_factor;
		ps_cb.object_material.z = entity->material.refraction_factor;
		ps_cb.object_material.w = entity->material.refraction_index;

		state->gpu->set_vertex_const_buffer(vs_cb);
		state->gpu->set_pixel_const_buffer(ps_cb);

		state->gpu->draw_vertex_buffer(entity->mesh);
	}
}
