#include "update/update_state.h"


void handle_camera(editor_state* state)
{
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
        camera.move_forward(state->timer.delta());
    }
    if (state->window->keyboard.s) {
        camera.move_back(state->timer.delta());
    }
    if (state->window->keyboard.d) {
        camera.move_right(state->timer.delta());
    }
    if (state->window->keyboard.a) {
        camera.move_left(state->timer.delta());
    }
    if (state->window->keyboard.e) {
        camera.move_up(state->timer.delta());
    }
    if (state->window->keyboard.q) {
        camera.move_down(state->timer.delta());
    }

    // Rotation
    if (state->window->mouse.right) {
        const kl::int2 frame_center = state->window->frame_center();
        if (state->window->mouse.is_hidden()) {
            camera.rotate(state->window->mouse.position(), frame_center);
        }
        else {
            state->window->mouse.set_hidden(true);
        }
        state->window->mouse.set_position(frame_center);
    }
    else if (state->window->mouse.is_hidden()) {
        state->window->mouse.set_hidden(false);
    }
}
