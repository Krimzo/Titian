#include "main.h"


titian::GUISectionSceneInfo::GUISectionSceneInfo(const LayerPackage& package)
    : GUISection("GUISectionSceneInfo", package)
{}

void titian::GUISectionSceneInfo::render_gui()
{
    const TimeBomb _ = this->time_it();

    if (imgui::Begin("Scene Info")) {
        Scene* scene = &game_layer->scene;

        int entity_count = static_cast<int>(scene->entity_count());
        imgui::DragInt("Entity count", &entity_count, 0.0f);

        kl::Float3 gravity = scene->gravity();
        if (imgui::DragFloat3("Gravity", gravity)) {
            scene->set_gravity(gravity);
        }

        imgui::Separator();
        main_camera_info(scene);

        imgui::Separator();
        main_ambient_info(scene);

        imgui::Separator();
        main_directional_info(scene);
    }
    imgui::End();
}

void titian::GUISectionSceneInfo::main_camera_info(Scene* scene)
{
    imgui::Text("Main Camera");

    std::string& bound_camera = scene->main_camera_name;
    if (imgui::BeginCombo("Bound Camera", bound_camera.c_str())) {
        const std::string filter = gui_input_continuous("Search###SceneInfoCamera");
        if (imgui::Selectable("/", bound_camera == "/")) {
            bound_camera = "/";
        }
        for (const auto& [entity_name, entity] : *scene) {
            if (!entity.is<const Camera>()) {
                continue;
            }
            if (!filter.empty() && entity_name.find(filter) == -1) {
                continue;
            }
            if (imgui::Selectable(entity_name.c_str(), entity_name == bound_camera)) {
                bound_camera = entity_name;
            }
        }
        imgui::EndCombo();
    }
}

void titian::GUISectionSceneInfo::main_ambient_info(Scene* scene)
{
    imgui::Text("Main Ambient Light");

    std::string& bound_light = scene->main_ambient_light_name;
    if (imgui::BeginCombo("Bound Ambient Light", bound_light.c_str())) {
        const std::string filter = gui_input_continuous("Search###SceneInfoAmbientLight");
        if (imgui::Selectable("/", bound_light == "/")) {
            bound_light = "/";
        }
        for (const auto& [entity_name, entity] : *scene) {
            if (!entity.is<const AmbientLight>()) {
                continue;
            }
            if (!filter.empty() && entity_name.find(filter) == -1) {
                continue;
            }
            if (imgui::Selectable(entity_name.c_str(), entity_name == bound_light)) {
                bound_light = entity_name;
            }
        }
        imgui::EndCombo();
    }
}

void titian::GUISectionSceneInfo::main_directional_info(Scene* scene)
{
    imgui::Text("Main Directional Light");

    std::string& bound_light = scene->main_directional_light_name;
    if (imgui::BeginCombo("Bound Directional Light", bound_light.c_str())) {
        const std::string filter = gui_input_continuous("Search###SceneInfoDirectionalLight");
        if (imgui::Selectable("/", bound_light == "/")) {
            bound_light = "/";
        }
        for (const auto& [entity_name, entity] : *scene) {
            if (!entity.is<const DirectionalLight>()) {
                continue;
            }
            if (!filter.empty() && entity_name.find(filter) == -1) {
                continue;
            }
            if (imgui::Selectable(entity_name.c_str(), entity_name == bound_light)) {
                bound_light = entity_name;
            }
        }
        imgui::EndCombo();
    }
}
