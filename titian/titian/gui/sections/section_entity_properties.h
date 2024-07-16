#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"

#include "scene/camera.h"
#include "light/ambient_light.h"
#include "light/point_light.h"
#include "light/directional_light.h"


namespace titian {
    class GUISectionEntityProperties : public GUISection
    {
    public:
        GUISectionEntityProperties(const LayerPackage& package);

        void render_gui() override;

    private:
        void display_entity_info(Scene* scene, const std::string& entity_name, Entity* entity);
        void display_camera_special_info(Scene* scene, Camera* camera);
        void display_ambient_light_special_info(Scene* scene, AmbientLight* light);
        void display_point_light_special_info(Scene* scene, PointLight* light);
        void display_directional_light_special_info(Scene* scene, DirectionalLight* light);

        void edit_entity_transform(Scene* scene, Entity* entity);
        void edit_entity_animation(Scene* scene, Entity* entity);
        void edit_entity_material(Scene* scene, Entity* entity);
        void edit_entity_physics(Scene* scene, const std::string& entity_name, kl::Object<Entity>& entity);
        void edit_entity_collider(Scene* scene, Entity* entity);
        void edit_entity_other(Scene* scene, Entity* entity);
    };
}
