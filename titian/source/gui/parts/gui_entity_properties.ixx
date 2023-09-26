export module gui_entity_properties;

export import gui_render;

void gui_entity_transform(EditorState* state, kl::Object<kl::Entity>& entity);
void gui_entity_physics(EditorState* state, kl::Object<kl::Entity>& entity);
void gui_entity_collider(EditorState* state, kl::Object<kl::Entity>& entity);
void gui_entity_mesh(EditorState* state, kl::Object<kl::Entity>& entity);
void gui_entity_material(EditorState* state, kl::Object<kl::Entity>& entity);

export void gui_entity_properties(EditorState* state)
{
    if (ImGui::Begin("Entity properties") && state->scene->selected_entity) {
        kl::Object<kl::Entity> entity = state->scene->selected_entity;
        gui_entity_transform(state, entity);
        gui_entity_mesh(state, entity);
        gui_entity_material(state, entity);
        gui_entity_physics(state, entity);
        gui_entity_collider(state, entity);
    }
    ImGui::End();
}

void gui_entity_transform(EditorState* state, kl::Object<kl::Entity>& entity)
{
    ImGui::Text("Transform");

    ImGui::DragFloat3("Scale", entity->render_scale);
    
    kl::Float3 rotation = entity->rotation();
    if (ImGui::DragFloat3("Rotation", rotation)) {
        entity->set_rotation(rotation);
    }

    kl::Float3 position = entity->position();
    if (ImGui::DragFloat3("Position", position)) {
        entity->set_position(position);
    }
}

void gui_entity_physics(EditorState* state, kl::Object<kl::Entity>& entity)
{
    ImGui::Separator();
    ImGui::Text("Physics");

    bool dynamic = entity->is_dynamic();
    if (ImGui::Checkbox("Dynamic", &dynamic)) {
        const std::string name = state->scene->find_name(entity);
        state->scene->remove(name);
        entity->set_dynamic(state->scene->physics(), dynamic);
        state->scene->add(name, entity);
    }

    if (dynamic) {
        bool gravity = entity->has_gravity();
        if (ImGui::Checkbox("Gravity", &gravity)) {
            entity->set_gravity(gravity);
        }

        float mass = entity->mass();
        if (ImGui::DragFloat("Mass", &mass, 1.0f, 0.0f, 1e9f)) {
            entity->set_mass(mass);
        }

        kl::Float3 velocity = entity->velocity();
        if (ImGui::DragFloat3("Velocity", velocity)) {
            entity->set_velocity(velocity);
        }

        kl::Float3 angular = entity->angular() * kl::TO_DEGREES;
        if (ImGui::DragFloat3("Angular", angular)) {
            entity->set_angular(angular * kl::TO_RADIANS);
        }
    }
}

void gui_entity_collider(EditorState* state, kl::Object<kl::Entity>& entity)
{
    static const std::unordered_map<physx::PxGeometryType::Enum, std::string> possible_colliders = {
        {     physx::PxGeometryType::Enum::eINVALID, "/" },

        {     physx::PxGeometryType::Enum::eBOX,     "box" },
        {  physx::PxGeometryType::Enum::eSPHERE,  "sphere" },
        { physx::PxGeometryType::Enum::eCAPSULE, "capsule" },
        
        {        physx::PxGeometryType::Enum::ePLANE, "plane" },
        { physx::PxGeometryType::Enum::eTRIANGLEMESH,  "mesh" },
    };

    ImGui::Separator();
    ImGui::Text("Collider");

    kl::Object<kl::Collider> collider = entity->collider();
    physx::PxGeometryType::Enum collider_type = (collider ? collider->type() : physx::PxGeometryType::Enum::eINVALID);
    std::string collider_name = possible_colliders.at(collider_type);

    // Choose type
    if (ImGui::BeginCombo("Bound Collider", collider_name.c_str())) {
        for (auto& [type, name] : possible_colliders) {
            if (ImGui::Selectable(name.c_str(), type == collider_type)) {
                collider = state->scene->make_default_collider(type, &entity->mesh);
                collider_type = (collider ? collider->type() : physx::PxGeometryType::Enum::eINVALID);
                collider_name = possible_colliders.at(collider_type);
                entity->set_collider(collider);
            }
        }

        ImGui::EndCombo();
    }
    if (!collider) {
        return;
    }

    // General info
    float restitution = collider->restitution();
    if (ImGui::DragFloat("Restitution", &restitution, 0.1f, 0.0f, 1e9f)) {
        collider->set_restitution(restitution);
    }

    float static_friction = collider->static_friction();
    if (ImGui::DragFloat("Static Friction", &static_friction, 0.1f, 0.0f, 1e9f)) {
        collider->set_static_friction(static_friction);
    }

    float dynamic_friction = collider->dynamic_friction();
    if (ImGui::DragFloat("Dynamic Friction", &dynamic_friction, 0.1f, 0.0f, 1e9f)) {
        collider->set_dynamic_friction(dynamic_friction);
    }

    // Specific info
    physx::PxBoxGeometry box_geometry = {};
    physx::PxTriangleMeshGeometry mesh_geometry = {};
    int geometry_type = 0;
    const auto collider_shape = collider->shape();
    if (collider_type == physx::PxGeometryType::Enum::eBOX) {
        collider_shape->getBoxGeometry(box_geometry);

        if (ImGui::DragFloat3("Size", (float*) &box_geometry.halfExtents, 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(box_geometry);
        }
        geometry_type = 1;
    }
    else if (collider_type == physx::PxGeometryType::Enum::eSPHERE) {
        physx::PxSphereGeometry geometry = {};
        collider_shape->getSphereGeometry(geometry);

        if (ImGui::DragFloat("Radius", &geometry.radius, 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(geometry);
        }
    }
    else if (collider_type == physx::PxGeometryType::Enum::eCAPSULE) {
        physx::PxCapsuleGeometry geometry = {};
        collider_shape->getCapsuleGeometry(geometry);

        if (ImGui::DragFloat("Radius", &geometry.radius, 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(geometry);
        }

        if (ImGui::DragFloat("Height", &geometry.halfHeight, 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(geometry);
        }
    }
    else if (collider_type == physx::PxGeometryType::Enum::eTRIANGLEMESH) {
        collider_shape->getTriangleMeshGeometry(mesh_geometry);

        if (ImGui::DragFloat3("Mesh Scale", (float*) &mesh_geometry.scale, 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(mesh_geometry);
        }
        geometry_type = 2;
    }

    kl::Float3 rotation = collider->rotation();
    if (ImGui::DragFloat3("Offset Rotation", rotation)) {
        collider->set_rotation(rotation);
    }

    kl::Float3 offset = collider->offset();
    if (ImGui::DragFloat3("Offset Position", offset)) {
        collider->set_offset(offset);
    }

    // Loading buttons
    if (geometry_type && ImGui::Button("Load size from scale")) {
        if (geometry_type == 1) {
            box_geometry.halfExtents = (physx::PxVec3&) entity->render_scale;
            collider_shape->setGeometry(box_geometry);
        }
        else {
            mesh_geometry.scale = (physx::PxVec3&) entity->render_scale;
            collider_shape->setGeometry(mesh_geometry);
        }
    }
}

void gui_entity_mesh(EditorState* state, kl::Object<kl::Entity>& entity)
{
    ImGui::Separator();
    ImGui::Text("Mesh");

    // Get name
    std::string bound_mesh_name = "?";
    for (const auto& [mesh_name, mesh] : state->scene->meshes) {
        if (mesh == entity->mesh) {
            bound_mesh_name = mesh_name;
            break;
        }
    }

    // Selector
    if (ImGui::BeginCombo("Bound Mesh", bound_mesh_name.c_str())) {
        if (ImGui::Selectable("/", !entity->mesh)) {
            entity->mesh = nullptr;
        }
        for (const auto& [mesh_name, mesh] : state->scene->meshes) {
            if (ImGui::Selectable(mesh_name.c_str(), mesh == entity->mesh)) {
                entity->mesh = mesh;
            }
        }
        ImGui::EndCombo();
    }
}

void gui_entity_material(EditorState* state, kl::Object<kl::Entity>& entity)
{
    ImGui::Separator();
    ImGui::Text("Material");

    // Get name
    std::string bound_material_name = "?";
    for (auto& [material_name, material] : state->scene->materials) {
        if (material == entity->material) {
            bound_material_name = material_name;
            break;
        }
    }

    // Selector
    if (ImGui::BeginCombo("Bound Material", bound_material_name.c_str())) {
        if (ImGui::Selectable("/", !entity->material)) {
            entity->material = nullptr;
        }
        for (auto& [material_name, material] : state->scene->materials) {
            if (ImGui::Selectable(material_name.c_str(), material == entity->material)) {
                entity->material = material;
            }
        }
        ImGui::EndCombo();
    }
}
