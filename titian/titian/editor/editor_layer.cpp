#include "main.h"


titian::EditorLayer::EditorLayer(GameLayer* game_layer)
{
    this->game_layer = game_layer;
}

bool titian::EditorLayer::update()
{
    Camera* camera = game_layer->scene->get_casted<Camera>(game_layer->scene->main_camera_name);
    if (!camera) { return true; }

    kl::Window* window = &game_layer->app_layer->window;
    const float delta_time = game_layer->app_layer->timer->delta();

    if (!is_viewport_focused || !is_over_viewport) {
        return true;
    }

    // Speed
    if (window->keyboard.shift) {
        camera->speed = 5.0f;
    }
    else {
        camera->speed = 2.0f;
    }

    // Movement
    if (window->keyboard.w) {
        camera->move_forward(delta_time);
    }
    if (window->keyboard.s) {
        camera->move_back(delta_time);
    }
    if (window->keyboard.d) {
        camera->move_right(delta_time);
    }
    if (window->keyboard.a) {
        camera->move_left(delta_time);
    }
    if (window->keyboard.e) {
        camera->move_up(delta_time);
    }
    if (window->keyboard.q) {
        camera->move_down(delta_time);
    }

    // Rotation
    if (window->mouse.right) {
        const kl::Int2 frame_center = window->frame_center();
        if (window->mouse.is_hidden()) {
            camera->rotate(window->mouse.position(), frame_center);
        }
        else {
            window->mouse.set_hidden(true);
        }
        window->mouse.set_position(frame_center);
    }
    else if (window->mouse.is_hidden()) {
        window->mouse.set_hidden(false);
    }
    return true;
}