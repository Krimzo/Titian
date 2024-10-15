#include "titian.h"


titian::GUISectionSceneInfo::GUISectionSceneInfo()
    : GUISection("GUISectionSceneInfo")
{}

void titian::GUISectionSceneInfo::render_gui()
{
    const TimeBomb _ = bench_time_bomb();

    if (im::Begin("Scene Info")) {
        Scene& scene = Layers::get<GameLayer>().scene();

        int entity_count = int(scene.entities().size());
        im::DragInt("Entity count", &entity_count, 0.0f);

        Float3 gravity = scene.gravity();
        if (im::DragFloat3("Gravity", &gravity.x)) {
            scene.set_gravity(gravity);
        }

        im::Separator();
        main_camera_info(scene);

        im::Separator();
        main_ambient_info(scene);

        im::Separator();
        main_directional_info(scene);
    }
    im::End();
}

void titian::GUISectionSceneInfo::main_camera_info(Scene& scene)
{
    im::Text("Main Camera");

    String& bound_camera = scene.main_camera_name;
    if (im::BeginCombo("Bound Camera", bound_camera.data())) {
        const String filter = gui_input_continuous("Search###SceneInfoCamera");
        if (im::Selectable("/", bound_camera == "/")) {
            bound_camera = "/";
        }
        for (const auto& [entity_name, entity] : scene.entities()) {
            if (!entity.is<const Camera>()) {
                continue;
            }
            if (!filter.empty() && entity_name.find(filter) == -1) {
                continue;
            }
            if (im::Selectable(entity_name.data(), entity_name == bound_camera)) {
                bound_camera = entity_name;
            }
        }
        im::EndCombo();
    }
}

void titian::GUISectionSceneInfo::main_ambient_info(Scene& scene)
{
    im::Text("Main Ambient Light");

    String& bound_light = scene.main_ambient_light_name;
    if (im::BeginCombo("Bound Ambient Light", bound_light.data())) {
        const String filter = gui_input_continuous("Search###SceneInfoAmbientLight");
        if (im::Selectable("/", bound_light == "/")) {
            bound_light = "/";
        }
        for (const auto& [entity_name, entity] : scene.entities()) {
            if (!entity.is<const AmbientLight>()) {
                continue;
            }
            if (!filter.empty() && entity_name.find(filter) == -1) {
                continue;
            }
            if (im::Selectable(entity_name.data(), entity_name == bound_light)) {
                bound_light = entity_name;
            }
        }
        im::EndCombo();
    }
}

void titian::GUISectionSceneInfo::main_directional_info(Scene& scene)
{
    im::Text("Main Directional Light");

    String& bound_light = scene.main_directional_light_name;
    if (im::BeginCombo("Bound Directional Light", bound_light.data())) {
        const String filter = gui_input_continuous("Search###SceneInfoDirectionalLight");
        if (im::Selectable("/", bound_light == "/")) {
            bound_light = "/";
        }
        for (const auto& [entity_name, entity] : scene.entities()) {
            if (!entity.is<const DirectionalLight>()) {
                continue;
            }
            if (!filter.empty() && entity_name.find(filter) == -1) {
                continue;
            }
            if (im::Selectable(entity_name.data(), entity_name == bound_light)) {
                bound_light = entity_name;
            }
        }
        im::EndCombo();
    }
}
