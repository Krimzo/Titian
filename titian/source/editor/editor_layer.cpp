#include "titian.h"


titian::EditorLayer::EditorLayer()
    : Layer("EditorLayer")
{}

void titian::EditorLayer::init()
{}

bool titian::EditorLayer::update()
{
    const TimeBomb _ = bench_time_bomb();

    AppLayer* app_layer = Layers::get<AppLayer>();
	GameLayer* game_layer = Layers::get<GameLayer>();
    
    if (!is_viewport_focused || !is_over_viewport) {
        return true;
    }

    Camera* camera = game_layer->scene->get_casted<Camera>(game_layer->scene->main_camera_name);
    if (!camera) {
        return true;
    }

    kl::Window* window = &app_layer->window;
    const float delta_time = app_layer->timer.delta();

    if (window->mouse.right) {
        const Int2 frame_center = window->frame_center();
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

    if (window->keyboard.shift) {
        camera->speed = 5.0f;
    }
    else if (window->keyboard.ctrl) {
        camera->speed = 1.0f;
    }
    else {
        camera->speed = 2.0f;
    }

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
    return true;
}
