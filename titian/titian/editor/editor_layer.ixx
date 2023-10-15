export module editor_layer;

export import game_layer;

export namespace titian {
    class EditorLayer : public Layer
    {
    public:
        kl::Object<GameLayer> game_layer = nullptr;
        
        bool game_running = false;
        bool is_viewport_focused = false;
        
        int gizmo_mode = 1;
        int gizmo_operation = 0;
        
        std::string selected_entity = "/";

        EditorLayer(kl::Object<GameLayer>& game_layer)
        {
            this->game_layer = game_layer;
        }

        ~EditorLayer() override
        {}

        bool update() override
        {
            Camera* camera = game_layer->scene->get_dynamic<Camera>(game_layer->scene->camera);
            if (!camera) { return true; }

            kl::Window* window = &game_layer->app_layer->window;
            const float delta_time = game_layer->app_layer->timer->delta();

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
    };
}
