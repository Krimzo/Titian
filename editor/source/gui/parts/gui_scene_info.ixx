export module gui_scene_info;

export import gui_render;

export void gui_scene_info(EditorState* state)
{
    if (ImGui::Begin("Scene Info")) {
        int entity_count = (int) state->scene->entity_count();
        ImGui::DragInt("Entity count", &entity_count);

        kl::Float3 gravity = state->scene->gravity();
        if (ImGui::DragFloat3("Gravity", gravity)) {
            state->scene->set_gravity(gravity);
        }

        if (kl::Object<kl::Camera>& camera = state->scene->camera) {
            ImGui::DragFloat3("Camera origin", camera->origin);

            kl::Float3 camera_direction = camera->forward();
            ImGui::DragFloat3("Camera direction", camera_direction);
            camera->set_forward(camera_direction);

            // Skybox
            std::string skybox_name = "/";
            for (const auto& [texture_name, texture] : state->scene->textures) {
                if (texture == camera->skybox) {
                    skybox_name = texture_name;
                    break;
                }
            }
            if (ImGui::BeginCombo("Bound Skybox", skybox_name.c_str())) {
                if (ImGui::Selectable("/", !camera->skybox)) {
                    camera->skybox = nullptr;
                }
                for (auto& [texture_name, texture] : state->scene->textures) {
                    if (ImGui::Selectable(texture_name.c_str(), texture == camera->skybox)) {
                        camera->skybox = texture;
                    }
                }
                ImGui::EndCombo();
            }

            // Background
            if (!camera->skybox) {
                kl::Float4 background = camera->background;
                if (ImGui::ColorEdit4("Background", background)) {
                    camera->background = background;
                }
            }
        }
    }
    ImGui::End();
}
