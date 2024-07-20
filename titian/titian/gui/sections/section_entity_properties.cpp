#include "main.h"


titian::GUISectionEntityProperties::GUISectionEntityProperties(const LayerPackage& package)
    : GUISection("GUISectionEntityProperties", package)
{}

void titian::GUISectionEntityProperties::render_gui()
{
    const TimeBomb _ = this->time_it();

    if (im::Begin("Entity properties") && !editor_layer->selected_entities.empty()) {
        Scene* scene = &game_layer->scene;
        const String entity_name = *--editor_layer->selected_entities.end();
        if (Ref entity = scene->get_entity(entity_name)) {
            display_entity_info(scene, entity_name, &entity);
            edit_entity_transform(scene, &entity);
            edit_entity_animation(scene, &entity);
            edit_entity_material(scene, &entity);
            edit_entity_physics(scene, entity_name, entity);
            edit_entity_collider(scene, &entity);
            edit_entity_other(scene, &entity);
        }
    }
    im::End();
}

void titian::GUISectionEntityProperties::display_entity_info(Scene* scene, const String& entity_name, Entity* entity)
{
    im::Text("Entity Info");

    im::Text("Name: ");
    im::SameLine();
    gui_colored_text(entity_name, gui_layer->special_color);

    if (Camera* camera = dynamic_cast<Camera*>(entity)) {
        im::Separator();
        display_camera_special_info(scene, camera);
    }
    else if (AmbientLight* light = dynamic_cast<AmbientLight*>(entity)) {
        im::Separator();
        display_ambient_light_special_info(scene, light);
    }
    else if (PointLight* light = dynamic_cast<PointLight*>(entity)) {
        im::Separator();
        display_point_light_special_info(scene, light);
    }
    else if (DirectionalLight* light = dynamic_cast<DirectionalLight*>(entity)) {
        im::Separator();
        display_directional_light_special_info(scene, light);
    }
}

void titian::GUISectionEntityProperties::display_camera_special_info(Scene* scene, Camera* camera)
{
    im::Text("Camera Special Info");

    if (im::BeginCombo("Camera Type", camera->type == CameraType::ORTHOGRAPHIC ? "Orthographic" : "Perspective")) {
        if (im::Selectable("Perspective", camera->type == CameraType::PERSPECTIVE)) {
            camera->type = (int) CameraType::PERSPECTIVE;
        }
        if (im::Selectable("Orthographic", camera->type == CameraType::ORTHOGRAPHIC)) {
            camera->type = (int) CameraType::ORTHOGRAPHIC;
        }
        im::EndCombo();
    }

    if (camera->type == CameraType::ORTHOGRAPHIC) {
        im::DragFloat("Width", &camera->width);
        im::DragFloat("Height", &camera->height);
    }
    else {
        im::DragFloat("Aspect Ratio", &camera->aspect_ratio);
        im::DragFloat("FOV", &camera->field_of_view);
    }

    im::DragFloat("Near Plane", &camera->near_plane);
    im::DragFloat("Far Plane", &camera->far_plane);

    im::DragFloat("Sensitivity", &camera->sensitivity);
    im::DragFloat("Speed", &camera->speed);

    Float3 camera_forward = camera->forward();
    im::DragFloat3("Forward", camera_forward, 0.01f);
    camera->set_forward(camera_forward);

    Float3 camera_up = camera->up();
    im::DragFloat3("Up", camera_up, 0.01f);
    camera->set_up(camera_up);

    // Skybox
    if (im::BeginCombo("Bound Skybox", camera->skybox_name.c_str())) {
        if (im::Selectable("/", camera->skybox_name == "/")) {
            camera->skybox_name = "/";
        }
        for (auto& [texture_name, _] : scene->textures) {
            if (im::Selectable(texture_name.c_str(), texture_name == camera->skybox_name)) {
                camera->skybox_name = texture_name;
            }
        }
        im::EndCombo();
    }

    // Shader
    if (im::BeginCombo("Bound Shader", camera->shader_name.c_str())) {
        if (im::Selectable("/", camera->shader_name == "/")) {
            camera->shader_name = "/";
        }
        for (auto& [shader_name, shader] : scene->shaders) {
            if (shader->type != ShaderType::CAMERA) {
                continue;
            }
            if (im::Selectable(shader_name.c_str(), shader_name == camera->shader_name)) {
                camera->shader_name = shader_name;
            }
        }
        im::EndCombo();
    }

    // Background
    if (!scene->textures.contains(camera->skybox_name)) {
        Float4 background = camera->background;
        if (im::ColorEdit4("Background", background)) {
            camera->background = background;
        }
    }
}

void titian::GUISectionEntityProperties::display_ambient_light_special_info(Scene* scene, AmbientLight* light)
{
    im::Text("Ambient Light Special Info");

    im::ColorEdit3("Color", light->color);
    im::DragFloat("Intensity", &light->intensity);
}

void titian::GUISectionEntityProperties::display_point_light_special_info(Scene* scene, PointLight* light)
{
    im::Text("Point Light Special Info");

    im::ColorEdit3("Color", light->color);
}

void titian::GUISectionEntityProperties::display_directional_light_special_info(Scene* scene, DirectionalLight* light)
{
    im::Text("Directional Light Special Info");

    im::DragFloat("Point Size", &light->point_size);
    im::ColorEdit3("Color", light->color);

    Float3 direction = light->direction();
    if (im::DragFloat3("Direction", direction)) {
        light->set_direction(direction);
    }
}

void titian::GUISectionEntityProperties::edit_entity_transform(Scene* scene, Entity* entity)
{
    im::Separator();
    im::Text("Transform");

    im::DragFloat3("Scale", entity->scale, 0.1f);

    Float3 rotation = entity->rotation();
    if (im::DragFloat3("Rotation", rotation)) {
        entity->set_rotation(rotation);
    }

    Float3 position = entity->position();
    if (im::DragFloat3("Position", position)) {
        entity->set_position(position);
    }
}

void titian::GUISectionEntityProperties::edit_entity_animation(Scene* scene, Entity* entity)
{
    im::Separator();
    im::Text("Animation");

    // Selector
    auto& bound_animation = entity->animation_name;
    if (im::BeginCombo("Bound Animation", bound_animation.c_str())) {
        const String filter = gui_input_continuous("Search###EntityPropsMesh");
        if (im::Selectable("/", bound_animation == "/")) {
            bound_animation = "/";
        }
        for (const auto& [name, animation] : scene->animations) {
            if (!filter.empty() && name.find(filter) == -1) {
                continue;
            }
            if (im::Selectable(name.c_str(), name == bound_animation)) {
                bound_animation = name;
            }
        }
        im::EndCombo();
    }
}

void titian::GUISectionEntityProperties::edit_entity_material(Scene* scene, Entity* entity)
{
    im::Separator();
    im::Text("Material");

    // Selector
    String& bound_material = entity->material_name;
    if (im::BeginCombo("Bound Material", bound_material.c_str())) {
        const String filter = gui_input_continuous("Search###EntityPropsMaterial");
        if (im::Selectable("/", bound_material == "/")) {
            bound_material = "/";
        }
        for (auto& [material, _] : scene->materials) {
            if (!filter.empty() && material.find(filter) == -1) {
                continue;
            }
            if (im::Selectable(material.c_str(), material == bound_material)) {
                bound_material = material;
            }
        }
        im::EndCombo();
    }
}

void titian::GUISectionEntityProperties::edit_entity_physics(Scene* scene, const String& entity_name, Ref<Entity>& entity)
{
    im::Separator();
    im::Text("Physics");

    bool dynamic = entity->is_dynamic();
    if (im::Checkbox("Dynamic", &dynamic)) {
        scene->remove_entity(entity_name);
        entity->set_dynamic(dynamic);
        scene->add_entity(entity_name, entity);
    }

    if (dynamic) {
        bool gravity = entity->has_gravity();
        if (im::Checkbox("Gravity", &gravity)) {
            entity->set_gravity(gravity);
        }

        float mass = entity->mass();
        if (im::DragFloat("Mass", &mass, 1.0f, 0.0f, 1e9f)) {
            entity->set_mass(mass);
        }

        Float3 velocity = entity->velocity();
        if (im::DragFloat3("Velocity", velocity)) {
            entity->set_velocity(velocity);
        }

        Float3 angular = entity->angular() * (float) kl::TO_DEGREES;
        if (im::DragFloat3("Angular", angular)) {
            entity->set_angular(angular * (float) kl::TO_RADIANS);
        }
    }
}

void titian::GUISectionEntityProperties::edit_entity_collider(Scene* scene, Entity* entity)
{
    static const Map<px::PxGeometryType::Enum, String> possible_colliders = {
        { px::PxGeometryType::Enum::eINVALID, "/" },
        { px::PxGeometryType::Enum::eBOX, "box" },
        { px::PxGeometryType::Enum::eSPHERE, "sphere" },
        { px::PxGeometryType::Enum::eCAPSULE, "capsule" },
    };

    im::Separator();
    im::Text("Collider");

    Ref collider = entity->collider();
    px::PxGeometryType::Enum collider_type = collider ? collider->type() : px::PxGeometryType::Enum::eINVALID;
    String collider_name = possible_colliders.contains(collider_type) ? possible_colliders.at(collider_type) : ("mesh_" + entity->collider_mesh_name);

    // Choose type
    if (im::BeginCombo("Bound Collider", collider_name.c_str())) {
        for (auto& [type, name] : possible_colliders) {
            if (im::Selectable(name.c_str(), type == collider_type)) {
                collider = scene->new_default_collider(type, nullptr);
                collider_type = (collider ? collider->type() : px::PxGeometryType::Enum::eINVALID);
                entity->set_collider(collider);
            }
        }
        for (auto& [name, mesh] : scene->meshes) {
            if (im::Selectable(("mesh_" + name).c_str(), entity->collider_mesh_name == name)) {
                entity->collider_mesh_name = name;

                Mesh* mesh = &scene->get_mesh(entity->collider_mesh_name);
                collider = scene->new_default_collider(px::PxGeometryType::Enum::eTRIANGLEMESH, mesh);
                collider_type = (collider ? collider->type() : px::PxGeometryType::Enum::eINVALID);
                entity->set_collider(collider);
            }
        }
        im::EndCombo();
    }
    if (!collider) {
        return;
    }

    // General info
    float restitution = collider->restitution();
    if (im::DragFloat("Restitution", &restitution, 0.1f, 0.0f, 1e9f)) {
        collider->set_restitution(restitution);
    }

    float static_friction = collider->static_friction();
    if (im::DragFloat("Static Friction", &static_friction, 0.1f, 0.0f, 1e9f)) {
        collider->set_static_friction(static_friction);
    }

    float dynamic_friction = collider->dynamic_friction();
    if (im::DragFloat("Dynamic Friction", &dynamic_friction, 0.1f, 0.0f, 1e9f)) {
        collider->set_dynamic_friction(dynamic_friction);
    }

    // Specific info
    int geometry_type = 0;
    px::PxBoxGeometry box_geometry = {};
    px::PxTriangleMeshGeometry mesh_geometry = {};
    px::PxShape* collider_shape = collider->shape();
    if (collider_type == px::PxGeometryType::Enum::eBOX) {
        collider_shape->getBoxGeometry(box_geometry);

        if (im::DragFloat3("Size", reinterpret_cast<float*>(&box_geometry.halfExtents), 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(box_geometry);
        }
        geometry_type = 1;
    }
    else if (collider_type == px::PxGeometryType::Enum::eSPHERE) {
        px::PxSphereGeometry geometry = {};
        collider_shape->getSphereGeometry(geometry);

        if (im::DragFloat("Radius", &geometry.radius, 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(geometry);
        }
    }
    else if (collider_type == px::PxGeometryType::Enum::eCAPSULE) {
        px::PxCapsuleGeometry geometry = {};
        collider_shape->getCapsuleGeometry(geometry);

        if (im::DragFloat("Radius", &geometry.radius, 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(geometry);
        }
        if (im::DragFloat("Height", &geometry.halfHeight, 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(geometry);
        }
    }
    else if (collider_type == px::PxGeometryType::Enum::eTRIANGLEMESH) {
        collider_shape->getTriangleMeshGeometry(mesh_geometry);

        if (im::DragFloat3("Mesh Scale", reinterpret_cast<float*>(&mesh_geometry.scale), 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(mesh_geometry);
        }
        geometry_type = 2;
    }

    Float3 rotation = collider->rotation();
    if (im::DragFloat3("Offset Rotation", rotation)) {
        collider->set_rotation(rotation);
    }

    Float3 offset = collider->offset();
    if (im::DragFloat3("Offset Position", offset)) {
        collider->set_offset(offset);
    }

    // Loading buttons
    if (geometry_type != 0 && im::Button("Load size from scale")) {
        if (geometry_type == 1) {
            box_geometry.halfExtents = reinterpret_cast<px::PxVec3&>(entity->scale);
            collider_shape->setGeometry(box_geometry);
        }
        else {
            mesh_geometry.scale = reinterpret_cast<px::PxVec3&>(entity->scale);
            collider_shape->setGeometry(mesh_geometry);
        }
    }
}

void titian::GUISectionEntityProperties::edit_entity_other(Scene* scene, Entity* entity)
{
    im::Separator();
    im::Text("Other Properties");

    im::Checkbox("Casts Shadows", &entity->casts_shadows);
}
