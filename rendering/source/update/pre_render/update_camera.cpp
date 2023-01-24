#include "update/update_state.h"


void handle_camera(state_machine* state)
{
	kl::camera& camera = state->scene->camera;
	camera.update_aspect_ratio(state->window->size());

	// Speed
	if (state->window->keyboard.shift.state()) {
		camera.speed = 5.0f;
	}
	else {
		camera.speed = 2.0f;
	}

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

		camera.rotate((kl::float2)position, (kl::float2)frame_center);
		state->window->mouse.set_position(frame_center);
	}
	else if (is_hidden) {
		state->window->mouse.set_hidden(false);
		is_hidden = false;
	}
}
