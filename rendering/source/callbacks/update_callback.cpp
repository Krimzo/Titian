#include "callbacks/callbacks.h"

#include "cbuffers/render.h"


void handle_resizes(state_machine* state);
void prepare_camera(state_machine* state);
void   update_light(state_machine* state);
void   render_scene(state_machine* state);
void finalize_frame(state_machine* state);

void update_callback(state_machine* state)
{
	handle_resizes(state);
	prepare_camera(state);
	update_light(state);

	state->gpu->clear_internal();
	render_scene(state);
	state->gpu->swap_buffers(true);

	finalize_frame(state);
}

void handle_resizes(state_machine* state)
{
	static kl::int2 last_size = {};
	const kl::int2 current_size = state->window->size();
	if (current_size.x > 0 && current_size.y > 0 && current_size != last_size) {
		state->gpu->resize_internal(current_size);
		state->gpu->set_viewport(current_size);
		last_size = current_size;
	}
}

void prepare_camera(state_machine* state)
{
	if (!state->scene) { return; }
	
	kl::camera& camera = state->scene->camera;
	camera.update_aspect_ratio(state->window->size());

	// Movement
	if (state->window->keyboard.w.state()) {
		camera.move_forward(state->timer.get_interval());
	}
	if (state->window->keyboard.s.state()) {
		camera.move_back(state->timer.get_interval());
	}
	if (state->window->keyboard.d.state()) {
		camera.move_right(state->timer.get_interval());
	}
	if (state->window->keyboard.a.state()) {
		camera.move_left(state->timer.get_interval());
	}
	if (state->window->keyboard.e.state()) {
		camera.move_up(state->timer.get_interval());
	}
	if (state->window->keyboard.q.state()) {
		camera.move_down(state->timer.get_interval());
	}

	// Rotation
	static bool is_hidden = false;
	if (state->window->mouse.right.state()) {
		const kl::int2 frame_center = state->window->get_frame_center();
		kl::int2 position = state->window->mouse.position();

		if (!is_hidden) {
			state->window->mouse.set_hidden(true);
			is_hidden = true;
			position = frame_center;
		}
		
		camera.rotate((kl::float2) position, (kl::float2) frame_center);
		state->window->mouse.set_position(frame_center);
	}
	else if (is_hidden) {
		state->window->mouse.set_hidden(false);
		is_hidden = false;
	}
}

void update_light(state_machine* state)
{
	if (!state->scene) { return; }

	const float elapsed_time = state->timer.get_elapsed();
	state->scene->sun_direction.x = cos(elapsed_time);
	state->scene->sun_direction.z = sin(elapsed_time);
}

void render_scene(state_machine* state)
{
	if (!state->scene) { return; }

	state->gpu->bind_shaders(state->shaders["render"]);

	render_vs_cb vs_cb = {};
	vs_cb.w_matrix = {};
	vs_cb.vp_matrix = state->scene->camera.matrix();
	vs_cb.vs_misc_data = {};

	render_ps_cb ps_cb = {};
	ps_cb.object_color = {};
	ps_cb.sun_direction = kl::float4(state->scene->sun_direction.normalize(), 0);
	ps_cb.ps_misc_data = {};
	
	for (auto& entity : *state->scene) {
		if (!entity->mesh) { continue; }

		vs_cb.w_matrix = entity->matrix();
		ps_cb.object_color = entity->color;

		state->gpu->set_vertex_const_buffer(vs_cb);
		state->gpu->set_pixel_const_buffer(ps_cb);
		state->gpu->draw_vertex_buffer(entity->mesh);
	}
}

void finalize_frame(state_machine* state)
{
	static constexpr int update_interval = 10;
	static size_t counter = 0;
	counter += 1;

	if (counter % update_interval == 0) {
		const int fps = (int) (1.0f / state->timer.get_interval());
		std::string title = kl::format("[", fps, "]");
		state->window->set_title(title);
	}
}
