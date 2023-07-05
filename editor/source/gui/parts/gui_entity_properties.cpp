#include "gui/gui_render.h"


void gui_entity_transform(editor_state* state, kl::object<kl::entity>& entity);
void gui_entity_physics(editor_state* state, kl::object<kl::entity>& entity);
void gui_entity_collider(editor_state* state, kl::object<kl::entity>& entity);
void gui_entity_mesh(editor_state* state, kl::object<kl::entity>& entity);
void gui_entity_material(editor_state* state, kl::object<kl::entity>& entity);

void gui_entity_properties(editor_state* state)
{
    if (ImGui::Begin("Entity properties") && state->scene->selected_entity) {
        kl::object<kl::entity> entity = state->scene->selected_entity;

        gui_entity_transform(state, entity);
        gui_entity_mesh(state, entity);
        gui_entity_material(state, entity);
        gui_entity_physics(state, entity);
        gui_entity_collider(state, entity);
    }
    ImGui::End();
}

void gui_entity_transform(editor_state* state, kl::object<kl::entity>& entity)
{
    ImGui::Text("Transform");

    ImGui::DragFloat3("Scale", entity->render_scale);
    
    const kl::float3 rotation = entity->get_rotation();
    if (ImGui::DragFloat3("Rotation", rotation)) {
        entity->set_rotation(rotation);
    }

    const kl::float3 position = entity->get_position();
    if (ImGui::DragFloat3("Position", position)) {
        entity->set_position(position);
    }
}

void gui_entity_physics(editor_state* state, kl::object<kl::entity>& entity)
{
    ImGui::Separator();
    ImGui::Text("Physics");

    bool dynamic = entity->is_dynamic();
    if (ImGui::Checkbox("Dynamic", &dynamic)) {
        const auto name = state->scene->get_name(entity);
        state->scene->remove(name);
        entity->set_dynamic(state->scene->get_physics(), dynamic);
        state->scene->add(name, entity);
    }

    if (dynamic) {
        bool gravity = entity->has_gravity();
        if (ImGui::Checkbox("Gravity", &gravity)) {
            entity->set_gravity(gravity);
        }

        float mass = entity->get_mass();
        if (ImGui::DragFloat("Mass", &mass, 1.0f, 0.0f, 1e9f)) {
            entity->set_mass(mass);
        }

        const kl::float3 velocity = entity->get_velocity();
        if (ImGui::DragFloat3("Velocity", velocity)) {
            entity->set_velocity(velocity);
        }

        const kl::float3 angular = entity->get_angular();
        if (ImGui::DragFloat3("Angular", angular)) {
            entity->set_angular(angular);
        }
    }
}

void gui_entity_collider(editor_state* state, kl::object<kl::entity>& entity)
{
    static const std::unordered_map<PxGeometryType::Enum, std::string> possible_colliders = {
        {     PxGeometryType::Enum::eINVALID, "" },

        {     PxGeometryType::Enum::eBOX,     "box" },
        {  PxGeometryType::Enum::eSPHERE,  "sphere" },
        { PxGeometryType::Enum::eCAPSULE, "capsule" },
        
        {        PxGeometryType::Enum::ePLANE, "plane" },
        { PxGeometryType::Enum::eTRIANGLEMESH,  "mesh" },
    };

    ImGui::Separator();
    ImGui::Text("Collider");

    kl::object<kl::collider> collider = entity->get_collider();
    PxGeometryType::Enum collider_type = (collider ? collider->get_type() : PxGeometryType::Enum::eINVALID);
    std::string collider_name = possible_colliders.at(collider_type);

    // Choose type
    if (ImGui::BeginCombo("Bound Collider", collider_name.c_str())) {
        for (auto& [type, name] : possible_colliders) {
            if (ImGui::Selectable(name.c_str(), type == collider_type)) {
                collider = state->scene->make_default_collider(type, &entity->mesh);
                collider_type = (collider ? collider->get_type() : PxGeometryType::Enum::eINVALID);
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
    float restitution = collider->get_restitution();
    if (ImGui::DragFloat("Restitution", &restitution, 0.1f, 0.0f, 1e9f)) {
        collider->set_restitution(restitution);
    }

    float static_friction = collider->get_static_friction();
    if (ImGui::DragFloat("Static Friction", &static_friction, 0.1f, 0.0f, 1e9f)) {
        collider->set_static_friction(static_friction);
    }

    float dynamic_friction = collider->get_dynamic_friction();
    if (ImGui::DragFloat("Dynamic Friction", &dynamic_friction, 0.1f, 0.0f, 1e9f)) {
        collider->set_dynamic_friction(dynamic_friction);
    }

    // Specific info
    PxBoxGeometry box_geometry = {};
    PxTriangleMeshGeometry mesh_geometry = {};
    int geometry_type = 0;
    const auto collider_shape = collider->get_shape();
    if (collider_type == PxGeometryType::Enum::eBOX) {
        collider_shape->getBoxGeometry(box_geometry);

        if (ImGui::DragFloat3("Size", (float*) &box_geometry.halfExtents, 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(box_geometry);
        }
        geometry_type = 1;
    }
    else if (collider_type == PxGeometryType::Enum::eSPHERE) {
        PxSphereGeometry geometry = {};
        collider_shape->getSphereGeometry(geometry);

        if (ImGui::DragFloat("Radius", &geometry.radius, 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(geometry);
        }
    }
    else if (collider_type == PxGeometryType::Enum::eCAPSULE) {
        PxCapsuleGeometry geometry = {};
        collider_shape->getCapsuleGeometry(geometry);

        if (ImGui::DragFloat("Radius", &geometry.radius, 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(geometry);
        }

        if (ImGui::DragFloat("Height", &geometry.halfHeight, 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(geometry);
        }
    }
    else if (collider_type == PxGeometryType::Enum::eTRIANGLEMESH) {
        collider_shape->getTriangleMeshGeometry(mesh_geometry);

        if (ImGui::DragFloat3("Mesh Scale", (float*) &mesh_geometry.scale, 0.5f, 0.0f, 1e9f)) {
            collider_shape->setGeometry(mesh_geometry);
        }
        geometry_type = 2;
    }

    const kl::float3 rotation = collider->get_rotation();
    if (ImGui::DragFloat3("Offset Rotation", rotation)) {
        collider->set_rotation(rotation);
    }

    const kl::float3 offset = collider->get_offset();
    if (ImGui::DragFloat3("Offset Position", offset)) {
        collider->set_offset(offset);
    }

    // Loading buttons
    if (geometry_type && ImGui::Button("Load scale from transform", { -1.0f, 0.0f })) {
        if (geometry_type == 1) {
            box_geometry.halfExtents = (PxVec3&) entity->render_scale;
            collider_shape->setGeometry(box_geometry);
        }
        else {
            mesh_geometry.scale = (PxVec3&) entity->render_scale;
            collider_shape->setGeometry(mesh_geometry);
        }
    }
}

void gui_entity_mesh(editor_state* state, kl::object<kl::entity>& entity)
{
    ImGui::Separator();
    ImGui::Text("Mesh");

    // Get name
    std::string bound_mesh_name = {};

    for (auto& [mesh_name, mesh] : state->default_meshes) {
        if (mesh == entity->mesh) {
            bound_mesh_name = mesh_name;
            break;
        }
    }

    if (bound_mesh_name.empty()) {
        for (auto& [mesh_name, mesh] : state->scene->meshes) {
            if (mesh == entity->mesh) {
                bound_mesh_name = mesh_name;
                break;
            }
        }
    }

    // Selector
    if (ImGui::BeginCombo("Bound Mesh", bound_mesh_name.c_str())) {
        if (ImGui::Selectable("", !entity->mesh)) {
            entity->mesh = nullptr;
        }

        for (auto& [mesh_name, mesh] : state->default_meshes) {
            if (ImGui::Selectable(mesh_name.c_str(), mesh == entity->mesh)) {
                entity->mesh = mesh;
            }
        }

        for (auto& [mesh_name, mesh] : state->scene->meshes) {
            if (ImGui::Selectable(mesh_name.c_str(), mesh == entity->mesh)) {
                entity->mesh = mesh;
            }
        }

        ImGui::EndCombo();
    }
}

void gui_entity_material(editor_state* state, kl::object<kl::entity>& entity)
{
    ImGui::Separator();
    ImGui::Text("Material");

    // Get name
    std::string bound_material_name = {};

    for (auto& [material_name, material] : state->default_materials) {
        if (material == entity->material) {
            bound_material_name = material_name;
            break;
        }
    }

    if (bound_material_name.empty()) {
        for (auto& [material_name, material] : state->scene->materials) {
            if (material == entity->material) {
                bound_material_name = material_name;
                break;
            }
        }
    }

    // Selector
    if (ImGui::BeginCombo("Bound Material", bound_material_name.c_str())) {
        if (ImGui::Selectable("", !entity->material)) {
            entity->material = nullptr;
        }

        for (auto& [material_name, material] : state->default_materials) {
            if (ImGui::Selectable(material_name.c_str(), material == entity->material)) {
                entity->material = material;
            }
        }

        for (auto& [material_name, material] : state->scene->materials) {
            if (ImGui::Selectable(material_name.c_str(), material == entity->material)) {
                entity->material = material;
            }
        }

        ImGui::EndCombo();
    }
}
