#include "main.h"


titian::GUISectionSceneInfo::GUISectionSceneInfo(EditorLayer* editor_layer)
    : GUISection("GUISectionSceneInfo")
    , editor_layer(editor_layer)
{}

void titian::GUISectionSceneInfo::render_gui()
{
    const TimeBomb _ = this->time_it();

    if (ImGui::Begin("Scene Info")) {
        Scene* scene = &editor_layer->game_layer->scene;

        int entity_count = static_cast<int>(scene->entity_count());
        ImGui::DragInt("Entity count", &entity_count, 0.0f);

        kl::Float3 gravity = scene->gravity();
        if (ImGui::DragFloat3("Gravity", gravity)) {
            scene->set_gravity(gravity);
        }

        ImGui::Separator();
        main_camera_info(scene);

        ImGui::Separator();
        main_ambient_info(scene);

        ImGui::Separator();
        main_directional_info(scene);
    }
    ImGui::End();
}

void titian::GUISectionSceneInfo::main_camera_info(Scene* scene)
{
    ImGui::Text("Main Camera");

    std::string& bound_camera = scene->main_camera_name;
    if (ImGui::BeginCombo("Bound Camera", bound_camera.c_str())) {
        const std::string filter = gui_input_continuous("Search###SceneInfoCamera");
        if (ImGui::Selectable("/", bound_camera == "/")) {
            bound_camera = "/";
        }
        for (const auto& [entity_name, entity] : *scene) {
            if (!dynamic_cast<const Camera*>(&entity)) {
                continue;
            }
            if (!filter.empty() && entity_name.find(filter) == -1) {
                continue;
            }
            if (ImGui::Selectable(entity_name.c_str(), entity_name == bound_camera)) {
                bound_camera = entity_name;
            }
        }
        ImGui::EndCombo();
    }
}

void titian::GUISectionSceneInfo::main_ambient_info(Scene* scene)
{
    ImGui::Text("Main Ambient Light");

    std::string& bound_light = scene->main_ambient_light_name;
    if (ImGui::BeginCombo("Bound Ambient Light", bound_light.c_str())) {
        const std::string filter = gui_input_continuous("Search###SceneInfoAmbientLight");
        if (ImGui::Selectable("/", bound_light == "/")) {
            bound_light = "/";
        }
        for (const auto& [entity_name, entity] : *scene) {
            if (!dynamic_cast<const AmbientLight*>(&entity)) {
                continue;
            }
            if (!filter.empty() && entity_name.find(filter) == -1) {
                continue;
            }
            if (ImGui::Selectable(entity_name.c_str(), entity_name == bound_light)) {
                bound_light = entity_name;
            }
        }
        ImGui::EndCombo();
    }
}

void titian::GUISectionSceneInfo::main_directional_info(Scene* scene)
{
    ImGui::Text("Main Directional Light");

    std::string& bound_light = scene->main_directional_light_name;
    if (ImGui::BeginCombo("Bound Directional Light", bound_light.c_str())) {
        const std::string filter = gui_input_continuous("Search###SceneInfoDirectionalLight");
        if (ImGui::Selectable("/", bound_light == "/")) {
            bound_light = "/";
        }
        for (const auto& [entity_name, entity] : *scene) {
            if (!dynamic_cast<const DirectionalLight*>(&entity)) {
                continue;
            }
            if (!filter.empty() && entity_name.find(filter) == -1) {
                continue;
            }
            if (ImGui::Selectable(entity_name.c_str(), entity_name == bound_light)) {
                bound_light = entity_name;
            }
        }
        ImGui::EndCombo();
    }
}
