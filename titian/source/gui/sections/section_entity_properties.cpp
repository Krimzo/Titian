#include "titian.h"


titian::GUISectionEntityProperties::GUISectionEntityProperties()
    : GUISection("GUISectionEntityProperties")
{}

void titian::GUISectionEntityProperties::render_gui()
{
    const TimeBomb _ = bench_time_bomb();

    EditorLayer* editor_layer = Layers::get<EditorLayer>();
	GameLayer* game_layer = Layers::get<GameLayer>();
    Scene* scene = &game_layer->scene;

    if (im::Begin("Entity properties") && !editor_layer->selected_entities.empty()) {
        const String entity_name = *(--editor_layer->selected_entities.end());
        Ref<Entity> entity;
        if (scene->entities().contains(entity_name)) {
            entity = scene->entities().at(entity_name);
        }
        if (entity) {
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

void titian::GUISectionEntityProperties::display_entity_info(Scene* scene, const StringView& entity_name, Entity* entity)
{
    GUILayer* gui_layer = Layers::get<GUILayer>();

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

    if (im::BeginCombo("Camera Type", camera->camera_type == CameraType::ORTHOGRAPHIC ? "Orthographic" : "Perspective")) {
        if (im::Selectable("Perspective", camera->camera_type == CameraType::PERSPECTIVE)) {
            camera->camera_type = (int) CameraType::PERSPECTIVE;
        }
        if (im::Selectable("Orthographic", camera->camera_type == CameraType::ORTHOGRAPHIC)) {
            camera->camera_type = (int) CameraType::ORTHOGRAPHIC;
        }
        im::EndCombo();
    }

    im::Checkbox("Enabled", &camera->enabled);
    im::Checkbox("vSync", &camera->v_sync);
    im::Checkbox("Wireframe", &camera->render_wireframe);
    
    Int2 resolution = camera->resolution();
    if (im::DragInt2("Resolution", &resolution.x, 1.0f, 1, 8192)) {
        camera->resize(resolution);
    }

    if (camera->camera_type == CameraType::ORTHOGRAPHIC) {
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
    im::DragFloat3("Forward", &camera_forward.x, 0.01f);
    camera->set_forward(camera_forward);

    Float3 camera_up = camera->up();
    im::DragFloat3("Up", &camera_up.x, 0.01f);
    camera->set_up(camera_up);

    if (im::BeginCombo("Bound Skybox", camera->skybox_name.data())) {
        if (im::Selectable("/", camera->skybox_name == "/")) {
            camera->skybox_name = "/";
        }
        for (const auto& [texture_name, _] : scene->textures) {
            if (im::Selectable(texture_name.data(), texture_name == camera->skybox_name)) {
                camera->skybox_name = texture_name;
            }
        }
        im::EndCombo();
    }
    if (im::BeginCombo("Bound Shader", camera->shader_name.data())) {
        if (im::Selectable("/", camera->shader_name == "/")) {
            camera->shader_name = "/";
        }
        for (const auto& [shader_name, shader] : scene->shaders) {
            if (shader->shader_type != ShaderType::CAMERA) {
                continue;
            }
            if (im::Selectable(shader_name.data(), shader_name == camera->shader_name)) {
                camera->shader_name = shader_name;
            }
        }
        im::EndCombo();
    }
    if (im::BeginCombo("Bound Target", camera->target_name.data())) {
        if (im::Selectable("/", camera->target_name == "/")) {
            camera->target_name = "/";
        }
        for (const auto& [texture_name, _] : scene->textures) {
            if (im::Selectable(texture_name.data(), texture_name == camera->target_name)) {
                camera->target_name = texture_name;
            }
        }
        im::EndCombo();
    }
    if (!scene->textures.contains(camera->skybox_name)) {
        Float4 background = camera->background;
        if (im::ColorEdit4("Background", &background.x)) {
            camera->background = background;
        }
    }
}

void titian::GUISectionEntityProperties::display_ambient_light_special_info(Scene* scene, AmbientLight* light)
{
    im::Text("Ambient Light Special Info");

    im::ColorEdit3("Color", &light->color.x);
    im::DragFloat("Intensity", &light->intensity, 0.01f);
}

void titian::GUISectionEntityProperties::display_point_light_special_info(Scene* scene, PointLight* light)
{
    im::Text("Point Light Special Info");

    im::ColorEdit3("Color", &light->color.x);
}

void titian::GUISectionEntityProperties::display_directional_light_special_info(Scene* scene, DirectionalLight* light)
{
    im::Text("Directional Light Special Info");

    im::ColorEdit3("Color", &light->color.x);
    im::DragFloat("Point Size", &light->point_size, 0.1f);

    for (int i = 0; i < DirectionalLight::CASCADE_COUNT; i++) {
        im::DragFloat2(kl::format("Cascade ", i).data(), light->cascade_splits + i, 0.01f, 0.0f, 1.0f);
    }
    std::sort(light->cascade_splits, light->cascade_splits + std::size(light->cascade_splits));

    int resolution = light->resolution();
	if (im::DragInt("Resolution", &resolution, 1.0f, 10, 8192)) {
		light->set_resolution(resolution);
	}

    Float3 direction = light->direction();
    if (im::DragFloat3("Direction", &direction.x, 0.01f)) {
        light->set_direction(direction);
    }
}

void titian::GUISectionEntityProperties::edit_entity_transform(Scene* scene, Entity* entity)
{
    im::Separator();
    im::Text("Transform");

	Float3 scale = entity->scale();
	if (im::DragFloat3("Scale", &scale.x, 0.1f)) {
		entity->set_scale(scale);
	}

    Float3 rotation = entity->rotation();
    if (im::DragFloat3("Rotation", &rotation.x)) {
        entity->set_rotation(rotation);
    }

    Float3 position = entity->position();
    if (im::DragFloat3("Position", &position.x)) {
        entity->set_position(position);
    }
}

void titian::GUISectionEntityProperties::edit_entity_animation(Scene* scene, Entity* entity)
{
    im::Separator();
    im::Text("Animation");
    auto& bound_animation = entity->animation_name;
    if (im::BeginCombo("Bound Animation", bound_animation.data())) {
        const String filter = gui_input_continuous("Search###EntityPropsMesh");
        if (im::Selectable("/", bound_animation == "/")) {
            bound_animation = "/";
        }
        for (const auto& [name, animation] : scene->animations) {
            if (!filter.empty() && name.find(filter) == -1) {
                continue;
            }
            if (im::Selectable(name.data(), name == bound_animation)) {
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

    String& bound_material = entity->material_name;
    if (im::BeginCombo("Bound Material", bound_material.data())) {
        const String filter = gui_input_continuous("Search###EntityPropsMaterial");
        if (im::Selectable("/", bound_material == "/")) {
            bound_material = "/";
        }
        for (auto& [material, _] : scene->materials) {
            if (!filter.empty() && material.find(filter) == -1) {
                continue;
            }
            if (im::Selectable(material.data(), material == bound_material)) {
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

    bool dynamic = entity->dynamic();
    if (im::Checkbox("Dynamic", &dynamic)) {
        scene->remove_entity(entity_name);
        entity->set_dynamic(dynamic);
        scene->add_entity(entity_name, entity);
    }

    if (dynamic) {
        bool gravity = entity->gravity();
        if (im::Checkbox("Gravity", &gravity)) {
            entity->set_gravity(gravity);
        }

        float mass = entity->mass();
        if (im::DragFloat("Mass", &mass, 1.0f, 0.0f, 1e9f)) {
            entity->set_mass(mass);
        }

        float angular_damping = entity->angular_damping();
		if (im::DragFloat("Angular Damping", &angular_damping, 0.01f, 0.0f, 1.0f)) {
			entity->set_angular_damping(angular_damping);
		}

        Float3 velocity = entity->velocity();
        if (im::DragFloat3("Velocity", &velocity.x)) {
            entity->set_velocity(velocity);
        }

        Float3 angular = entity->angular() * kl::to_degrees();
        if (im::DragFloat3("Angular", &angular.x)) {
            entity->set_angular(angular * kl::to_radians());
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

    if (im::BeginCombo("Bound Collider", collider_name.data())) {
        for (auto& [type, name] : possible_colliders) {
            if (im::Selectable(name.data(), type == collider_type)) {
                collider = scene->new_default_collider(type, nullptr);
                collider_type = (collider ? collider->type() : px::PxGeometryType::Enum::eINVALID);
                entity->set_collider(collider);
            }
        }
        for (auto& [name, mesh] : scene->meshes) {
            if (im::Selectable(("mesh_" + name).data(), entity->collider_mesh_name == name)) {
                entity->collider_mesh_name = name;

                Mesh* mesh = scene->helper_get_mesh(entity->collider_mesh_name);
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

    int geometry_type = 0;
    px::PxBoxGeometry box_geometry{};
    px::PxTriangleMeshGeometry mesh_geometry{};
    px::PxShape* collider_shape = collider->shape();
    if (collider_type == px::PxGeometryType::Enum::eBOX) {
        collider_shape->getBoxGeometry(box_geometry);
        Float3 box_size = px_cast(box_geometry.halfExtents) * 2.0f;
        if (im::DragFloat3("Size", &box_size.x, 0.5f, 0.0f, 1e9f)) {
            box_geometry.halfExtents = px_cast(box_size) * 0.5f;
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

        if (im::DragFloat3("Mesh Scale", &mesh_geometry.scale.scale.x, 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(mesh_geometry);
        }
        geometry_type = 2;
    }

    Float3 rotation = collider->rotation();
    if (im::DragFloat3("Offset Rotation", &rotation.x)) {
        collider->set_rotation(rotation);
    }

    Float3 offset = collider->offset();
    if (im::DragFloat3("Offset Position", &offset.x)) {
        collider->set_offset(offset);
    }

    if (geometry_type != 0 && im::Button("Load size from scale")) {
        const Float3 scale = entity->scale();
        if (geometry_type == 1) {
            box_geometry.halfExtents = px_cast(scale) * 0.5f;
            collider_shape->setGeometry(box_geometry);
        }
        else {
            mesh_geometry.scale = px_cast(scale);
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
