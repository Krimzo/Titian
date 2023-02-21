#include "update/update_state.h"


void handle_camera(state_machine* state)
{
    if (!state->gui_state.is_viewport_focused) { return; }

    kl::camera& camera = *state->scene->camera;

    // Speed
    if (state->window->keyboard.shift) {
        camera.speed = 5.0f;
    }
    else {
        camera.speed = 2.0f;
    }

    // Movement
    if (state->window->keyboard.w) {
        camera.move_forward(state->timer.get_interval());
    }
    if (state->window->keyboard.s) {
        camera.move_back(state->timer.get_interval());
    }
    if (state->window->keyboard.d) {
        camera.move_right(state->timer.get_interval());
    }
    if (state->window->keyboard.a) {
        camera.move_left(state->timer.get_interval());
    }
    if (state->window->keyboard.e) {
        camera.move_up(state->timer.get_interval());
    }
    if (state->window->keyboard.q) {
        camera.move_down(state->timer.get_interval());
    }

    // Rotation
    static bool is_hidden = false;
    if (state->window->mouse.right) {
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
