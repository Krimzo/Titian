#pragma once

#include "editor/editor_layer.h"
#include "gui/gui_layer.h"
#include "gui/gui_section.h"
#include "scene/light/ambient_light.h"
#include "scene/light/directional_light.h"
#include "scene/camera.h"


namespace titian {
    struct GUISectionEntityProperties : GUISection
    {
        GUISectionEntityProperties();

        void render_gui() override;

    private:
        void display_entity_info(Scene& scene, const StringRef& entity_name, Entity& entity);
        void display_camera_special_info(Scene& scene, Camera& camera);
        void display_ambient_light_special_info(Scene& scene, AmbientLight& light);
        void display_directional_light_special_info(Scene& scene, DirectionalLight& light);

        void edit_entity_transform(Scene& scene, Entity& entity);
        void edit_entity_animation(Scene& scene, Entity& entity);
        void edit_entity_material(Scene& scene, Entity& entity);
        void edit_entity_physics(Scene& scene, Entity& entity);
        void edit_entity_collider(Scene& scene, Entity& entity);
        void edit_entity_properties(Scene& scene, Entity& entity);
    };
}
