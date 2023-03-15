#include "gui/gui_render.h"


void gui_entity_transform(state_machine* state, kl::ref<kl::entity> entity);
void gui_entity_physics(state_machine* state, kl::ref<kl::entity> entity);
void gui_entity_collider(state_machine* state, kl::ref<kl::entity> entity);
void gui_entity_mesh(state_machine* state, kl::ref<kl::entity> entity);
void gui_entity_material(state_machine* state, kl::ref<kl::entity> entity);

void gui_entity_properties(state_machine* state)
{
    if (ImGui::Begin("Entity properties") && state->scene->selected_entity) {
        kl::ref<kl::entity> entity = state->scene->selected_entity;

        gui_entity_transform(state, entity);
        gui_entity_mesh(state, entity);
        gui_entity_material(state, entity);
        gui_entity_physics(state, entity);
        gui_entity_collider(state, entity);
        
        ImGui::Separator();
    }
    ImGui::End();
}

void gui_entity_transform(state_machine* state, kl::ref<kl::entity> entity)
{
    ImGui::Separator();
    ImGui::Text("Transform");

    ImGui::DragFloat3("Scale", entity->render_scale);

    kl::float3 rotation = entity->get_rotation();
    if (ImGui::DragFloat3("Rotation", rotation)) {
        entity->set_rotation(rotation);
    }

    kl::float3 position = entity->get_position();
    if (ImGui::DragFloat3("Position", position)) {
        entity->set_position(position);
    }
}

void gui_entity_physics(state_machine* state, kl::ref<kl::entity> entity)
{
    ImGui::Separator();
    ImGui::Text("Physics");

    bool dynamic = entity->is_dynamic();
    if (ImGui::Checkbox("Dynamic", &dynamic)) {
        const auto name = state->scene->get_name(entity);
        state->scene->remove(name);
        entity->set_dynamic(dynamic);
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

        kl::float3 velocity = entity->get_velocity();
        if (ImGui::DragFloat3("Velocity", velocity)) {
            entity->set_velocity(velocity);
        }

        kl::float3 angular = entity->get_angular();
        if (ImGui::DragFloat3("Angular", angular)) {
            entity->set_angular(angular);
        }
    }
}

void gui_entity_collider(state_machine* state, kl::ref<kl::entity> entity)
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

    kl::ref<kl::collider> collider = entity->get_collider();
    PxGeometryType::Enum collider_type = (collider ? collider->get_type() : PxGeometryType::Enum::eINVALID);
    std::string collider_name = possible_colliders.at(collider_type);

    // General info
    if (collider) {
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
    }

    // Choose type
    if (ImGui::BeginCombo("Bound Collider", collider_name.c_str())) {
        for (auto& [type, name] : possible_colliders) {
            if (ImGui::Selectable(name.c_str(), type == collider_type)) {
                collider = state->scene->make_default_collider(type, entity->mesh.get());
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

    // Specific info
    auto collider_shape = collider->get_shape();
    PxTransform transform = collider_shape->getLocalPose();

    if (collider_type == PxGeometryType::Enum::eBOX) {
        PxBoxGeometry geometry = {};
        collider_shape->getBoxGeometry(geometry);

        if (ImGui::DragFloat3("Collider Size", (float*) &geometry.halfExtents)) {
            collider_shape->setGeometry(geometry);
        }
    }
    else if (collider_type == PxGeometryType::Enum::eSPHERE) {
        PxSphereGeometry geometry = {};
        collider_shape->getSphereGeometry(geometry);

        if (ImGui::DragFloat("Collider Radius", &geometry.radius)) {
            collider_shape->setGeometry(geometry);
        }
    }
    else if (collider_type == PxGeometryType::Enum::eCAPSULE) {
        PxCapsuleGeometry geometry = {};
        collider_shape->getCapsuleGeometry(geometry);

        if (ImGui::DragFloat("Collider Radius", &geometry.radius)) {
            collider_shape->setGeometry(geometry);
        }

        if (ImGui::DragFloat("Collider Height", &geometry.halfHeight)) {
            collider_shape->setGeometry(geometry);
        }
    }
    else if (collider_type == PxGeometryType::Enum::eTRIANGLEMESH) {
        PxTriangleMeshGeometry geometry = {};
        collider_shape->getTriangleMeshGeometry(geometry);

        if (ImGui::DragFloat3("Collider Scale", (float*) &geometry.scale)) {
            collider_shape->setGeometry(geometry);
        }
    }

    kl::float3 rotation = collider->get_rotation();
    if (ImGui::DragFloat3("Collider Rotation", rotation)) {
        collider->set_rotation(rotation);
    }

    kl::float3 offset = collider->get_offset();
    if (ImGui::DragFloat3("Collider Offset", offset)) {
        collider->set_offset(offset);
    }
}

void gui_entity_mesh(state_machine* state, kl::ref<kl::entity> entity)
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

void gui_entity_material(state_machine* state, kl::ref<kl::entity> entity)
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
