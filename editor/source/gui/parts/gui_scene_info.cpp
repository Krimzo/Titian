#include "gui/gui_render.h"


void gui_scene_info(editor_state* state)
{
    if (ImGui::Begin("Scene Info")) {
        int entity_count = (int) state->scene->entity_count();
        ImGui::DragInt("Entity count", &entity_count);

        kl::float3 gravity = state->scene->gravity();
        if (ImGui::DragFloat3("Gravity", gravity)) {
            state->scene->set_gravity(gravity);
        }

        if (kl::object<kl::camera>& camera = state->scene->camera) {
            ImGui::DragFloat3("Camera origin", camera->origin);

            const kl::float3 camera_direction = camera->forward();
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
                kl::float4 background = camera->background;
                if (ImGui::ColorEdit4("Background", background)) {
                    camera->background = background;
                }
            }
        }
    }
    ImGui::End();
}
