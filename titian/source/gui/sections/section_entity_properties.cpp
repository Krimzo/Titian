#include "titian.h"


titian::GUISectionEntityProperties::GUISectionEntityProperties()
    : GUISection("GUISectionEntityProperties")
{}

void titian::GUISectionEntityProperties::render_gui()
{
    const TimeBomb _ = bench_time_bomb();

    EditorLayer& editor_layer = EditorLayer::get();
	GameLayer& game_layer = GameLayer::get();
    Scene& scene = game_layer.scene();

    if (im::Begin("Entity properties") && !editor_layer.selected_entities.empty()) {
        const String entity_name = *(--editor_layer.selected_entities.end());
        Ref<Entity> entity;
        if (scene.entities().contains(entity_name)) {
            entity = scene.entities().at(entity_name);
        }
        if (entity) {
            display_entity_info(scene, entity_name, *entity);
            edit_entity_transform(scene, *entity);
            edit_entity_animation(scene, *entity);
            edit_entity_material(scene, *entity);
            edit_entity_physics(scene, *entity);
            edit_entity_collider(scene, *entity);
            edit_entity_properties(scene, *entity);
        }
    }
    im::End();
}

void titian::GUISectionEntityProperties::display_entity_info(Scene& scene, const StringRef& entity_name, Entity& entity)
{
    GUILayer& gui_layer = GUILayer::get();

    im::Text("Entity Info");

    im::Text("Name: ");
    im::SameLine();
    gui_colored_text(entity_name, gui_layer.special_color);

    if (Camera* camera = dynamic_cast<Camera*>(&entity)) {
        im::Separator();
        display_camera_special_info(scene, *camera);
    }
    else if (AmbientLight* light = dynamic_cast<AmbientLight*>(&entity)) {
        im::Separator();
        display_ambient_light_special_info(scene, *light);
    }
    else if (DirectionalLight* light = dynamic_cast<DirectionalLight*>(&entity)) {
        im::Separator();
        display_directional_light_special_info(scene, *light);
    }
}

void titian::GUISectionEntityProperties::display_camera_special_info(Scene& scene, Camera& camera)
{
    im::Text("Camera Special Info");

    if (im::BeginCombo("Camera Type", camera.camera_type == CameraType::ORTHOGRAPHIC ? "Orthographic" : "Perspective")) {
        if (im::Selectable("Perspective", camera.camera_type == CameraType::PERSPECTIVE)) {
            camera.camera_type = CameraType::PERSPECTIVE;
        }
        if (im::Selectable("Orthographic", camera.camera_type == CameraType::ORTHOGRAPHIC)) {
            camera.camera_type = CameraType::ORTHOGRAPHIC;
        }
        im::EndCombo();
    }

    im::Checkbox("Enabled", &camera.enabled);
    im::Checkbox("vSync", &camera.v_sync);
    im::Checkbox("Wireframe", &camera.render_wireframe);
    
    Int2 resolution = camera.resolution();
    if (im::DragInt2("Resolution", &resolution.x, 1.0f, 1, 8192)) {
        camera.resize(resolution);
    }

    if (camera.camera_type == CameraType::ORTHOGRAPHIC) {
        im::DragFloat("Width", &camera.width);
        im::DragFloat("Height", &camera.height);
    }
    else {
        im::DragFloat("Aspect Ratio", &camera.aspect_ratio);
        im::DragFloat("FOV", &camera.field_of_view);
    }

    im::DragFloat("Near Plane", &camera.near_plane);
    im::DragFloat("Far Plane", &camera.far_plane);

    im::DragFloat("Sensitivity", &camera.sensitivity);
    im::DragFloat("Speed", &camera.speed);

    Float3 camera_forward = camera.forward();
    im::DragFloat3("Forward", &camera_forward.x, 0.01f);
    camera.set_forward(camera_forward);

    Float3 camera_up = camera.up();
    im::DragFloat3("Up", &camera_up.x, 0.01f);
    camera.set_up(camera_up);

    if (im::BeginCombo("Bound Skybox", camera.skybox_texture_name.data())) {
        if (im::Selectable("/", camera.skybox_texture_name == "/")) {
            camera.skybox_texture_name = "/";
        }
        for (const auto& [texture_name, _] : scene.textures) {
            if (im::Selectable(texture_name.data(), texture_name == camera.skybox_texture_name)) {
                camera.skybox_texture_name = texture_name;
            }
        }
        im::EndCombo();
    }
    if (im::BeginCombo("Bound Shader", camera.shader_name.data())) {
        if (im::Selectable("/", camera.shader_name == "/")) {
            camera.shader_name = "/";
        }
        for (const auto& [shader_name, shader] : scene.shaders) {
            if (shader->shader_type != ShaderType::CAMERA) {
                continue;
            }
            if (im::Selectable(shader_name.data(), shader_name == camera.shader_name)) {
                camera.shader_name = shader_name;
            }
        }
        im::EndCombo();
    }
    if (im::BeginCombo("Bound Target", camera.target_texture_name.data())) {
        if (im::Selectable("/", camera.target_texture_name == "/")) {
            camera.target_texture_name = "/";
        }
        for (const auto& [texture_name, _] : scene.textures) {
            if (im::Selectable(texture_name.data(), texture_name == camera.target_texture_name)) {
                camera.target_texture_name = texture_name;
            }
        }
        im::EndCombo();
    }
    if (!scene.textures.contains(camera.skybox_texture_name)) {
        Float4 background = camera.background;
        if (im::ColorEdit4("Background", &background.x)) {
            camera.background = background;
        }
    }
}

void titian::GUISectionEntityProperties::display_ambient_light_special_info(Scene& scene, AmbientLight& light)
{
    im::Text("Ambient Light Special Info");
    im::ColorEdit3("Color", &light.color.x);
}

void titian::GUISectionEntityProperties::display_directional_light_special_info(Scene& scene, DirectionalLight& light)
{
    im::Text("Directional Light Special Info");

    im::ColorEdit3("Color", &light.color.x);
    im::DragFloat("Point Size", &light.point_size, 0.01f, 0.0f, 1.0f);

    for (int i = 0; i < DirectionalLight::CASCADE_COUNT; i++) {
        im::DragFloat2(kl::format("Cascade ", i).data(), light.cascade_splits + i, 0.01f, 0.0f, 1.0f);
    }
    std::sort(light.cascade_splits, light.cascade_splits + std::size(light.cascade_splits));

    int resolution = light.resolution();
	if (im::DragInt("Resolution", &resolution, 1.0f, 10, 8192)) {
		light.set_resolution(resolution);
	}

    Float3 direction = light.direction();
    if (im::DragFloat3("Direction", &direction.x, 0.01f)) {
        light.set_direction(direction);
    }
}

void titian::GUISectionEntityProperties::edit_entity_transform(Scene& scene, Entity& entity)
{
    im::Separator();
    im::Text("Transform");

	Float3 scale = entity.scale();
	if (im::DragFloat3("Scale", &scale.x, 0.1f)) {
		entity.set_scale(scale);
	}

    Float3 rotation = entity.rotation();
    if (im::DragFloat3("Rotation", &rotation.x)) {
        entity.set_rotation(rotation);
    }

    Float3 position = entity.position();
    if (im::DragFloat3("Position", &position.x)) {
        entity.set_position(position);
    }
}

void titian::GUISectionEntityProperties::edit_entity_animation(Scene& scene, Entity& entity)
{
    im::Separator();
    im::Text("Animation");
    auto& bound_animation = entity.animation_name;
    if (im::BeginCombo("Bound Animation", bound_animation.data())) {
        const String filter = gui_input_continuous("Search###EntityPropsMesh");
        if (im::Selectable("/", bound_animation == "/")) {
            bound_animation = "/";
        }
        for (const auto& [name, animation] : scene.animations) {
            if (!filter.empty() && !str_find(name, filter)) {
                continue;
            }
            if (im::Selectable(name.data(), name == bound_animation)) {
                bound_animation = name;
            }
        }
        im::EndCombo();
    }
}

void titian::GUISectionEntityProperties::edit_entity_material(Scene& scene, Entity& entity)
{
    im::Separator();
    im::Text("Material");

    String& bound_material = entity.material_name;
    if (im::BeginCombo("Bound Material", bound_material.data())) {
        const String filter = gui_input_continuous("Search###EntityPropsMaterial");
        if (im::Selectable("/", bound_material == "/")) {
            bound_material = "/";
        }
        for (auto& [material, _] : scene.materials) {
            if (!filter.empty() && !str_find(material, filter)) {
                continue;
            }
            if (im::Selectable(material.data(), material == bound_material)) {
                bound_material = material;
            }
        }
        im::EndCombo();
    }
}

void titian::GUISectionEntityProperties::edit_entity_physics(Scene& scene, Entity& entity)
{
    im::Separator();
    im::Text("Physics");

    bool dynamic = entity.dynamic();
    if (im::Checkbox("Dynamic", &dynamic)) {
        entity.set_dynamic(dynamic);
    }

    bool gravity = entity.gravity();
    if (im::Checkbox("Gravity", &gravity)) {
        entity.set_gravity(gravity);
    }

    float mass = entity.mass();
    if (im::DragFloat("Mass", &mass, 1.0f, 0.0f, 1e9f)) {
        entity.set_mass(mass);
    }

    float angular_damping = entity.angular_damping();
    if (im::DragFloat("Damping", &angular_damping, 0.01f, 0.0f, 1.0f)) {
        entity.set_angular_damping(angular_damping);
    }

    Float3 velocity = entity.velocity();
    if (im::DragFloat3("Velocity", &velocity.x)) {
        entity.set_velocity(velocity);
    }

    Float3 angular = entity.angular() * kl::to_degrees();
    if (im::DragFloat3("Angular", &angular.x)) {
        entity.set_angular(angular * kl::to_radians());
    }
}

void titian::GUISectionEntityProperties::edit_entity_collider(Scene& scene, Entity& entity)
{
    static const Map<px::PxGeometryType::Enum, String> possible_geometries = {
        { px::PxGeometryType::Enum::eINVALID, "/" },
        { px::PxGeometryType::Enum::eBOX, "box" },
        { px::PxGeometryType::Enum::eSPHERE, "sphere" },
        { px::PxGeometryType::Enum::eCAPSULE, "capsule" },
    };

    im::Separator();
    im::Text("Collider");

    px::PxGeometryType::Enum geometry_type = entity.geometry_type();
    String collider_name = possible_geometries.contains(geometry_type) ? possible_geometries.at(geometry_type) : "/";

    if (im::BeginCombo("Bound Collider", collider_name.data())) {
        for (auto& [type, name] : possible_geometries) {
            if (im::Selectable(name.data(), type == geometry_type)) {
                geometry_type = type;
                switch (type)
                {
                case px::PxGeometryType::Enum::eINVALID: entity.remove_collider(); break;
                case px::PxGeometryType::Enum::eBOX: entity.set_box_collider({ 1.0f, 1.0f, 1.0f }); break;
                case px::PxGeometryType::Enum::eSPHERE: entity.set_sphere_collider(1.0f); break;
				case px::PxGeometryType::Enum::eCAPSULE: entity.set_capsule_collider(1.0f, 2.0f); break;
                }
            }
        }
        im::EndCombo();
    }

    float restitution = entity.restitution();
    if (im::DragFloat("Restitution", &restitution, 0.1f, 0.0f, 1e9f)) {
        entity.set_restitution(restitution);
    }

    float static_friction = entity.static_friction();
    if (im::DragFloat("Static Friction", &static_friction, 0.1f, 0.0f, 1e9f)) {
        entity.set_static_friction(static_friction);
    }

    float dynamic_friction = entity.dynamic_friction();
    if (im::DragFloat("Dynamic Friction", &dynamic_friction, 0.1f, 0.0f, 1e9f)) {
        entity.set_dynamic_friction(dynamic_friction);
    }

    const auto geometry = entity.collider_geometry();
    switch (geometry.getType())
    {
    case px::PxGeometryType::Enum::eBOX:
    {
        px::PxBoxGeometry box_geometry = geometry.box();
        box_geometry.halfExtents *= 2.0f;
        if (im::DragFloat3("Box Size", &box_geometry.halfExtents.x, 0.1f, 0.0f, 1e9f)) {
            box_geometry.halfExtents *= 0.5f;
            entity.set_collider_geometry(box_geometry);
        }
        break;
    }
    case px::PxGeometryType::Enum::eSPHERE:
    {
        px::PxSphereGeometry sphere_geometry = geometry.sphere();
        if (im::DragFloat("Sphere Radius", &sphere_geometry.radius, 0.1f, 0.0f, 1e9f)) {
			entity.set_collider_geometry(sphere_geometry);
        }
        break;
    }
    case px::PxGeometryType::Enum::eCAPSULE:
    {
        px::PxCapsuleGeometry capsule_geometry = geometry.capsule();
        if (im::DragFloat("Capsule Radius", &capsule_geometry.radius, 0.5f, 0.0f, 1e9f)) {
            entity.set_collider_geometry(capsule_geometry);
        }
        if (im::DragFloat("Capsule Height", &capsule_geometry.halfHeight, 0.5f, 0.0f, 1e9f)) {
            entity.set_collider_geometry(capsule_geometry);
        }
        break;
    }
    }

    Float3 collider_rotation = entity.collider_rotation();
    if (im::DragFloat3("Collider Rotation", &collider_rotation.x)) {
        entity.set_collider_rotation(collider_rotation);
    }

    Float3 collider_offset = entity.collider_offset();
    if (im::DragFloat3("Collider Offset", &collider_offset.x)) {
        entity.set_collider_offset(collider_offset);
    }

    if (geometry_type == px::PxGeometryType::Enum::eBOX && im::Button("Copy scale")) {
        entity.set_box_collider(entity.scale());
    }
}

void titian::GUISectionEntityProperties::edit_entity_properties(Scene& scene, Entity& entity)
{
    im::Separator();
    im::Text("Properties");

    im::Checkbox("Shadows", &entity.shadows);
}
