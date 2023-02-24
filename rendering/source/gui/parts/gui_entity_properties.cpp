#include "gui/gui_render.h"


void gui_entity_properties(state_machine* state)
{
    if (ImGui::Begin("Entity properties") && state->scene->selected_entity) {
        kl::entity& entity = *state->scene->selected_entity;

        // Transform
        ImGui::Separator();
        ImGui::Text("Transform");

        ImGui::DragFloat3("Scale", entity.render_scale);

        kl::float3 rotation = entity.get_rotation();
        if (ImGui::DragFloat3("Rotation", rotation)) {
            entity.set_rotation(rotation);
        }

        kl::float3 position = entity.get_position();
        if (ImGui::DragFloat3("Position", position)) {
            entity.set_position(position);
        }

        // Physics
        ImGui::Separator();
        ImGui::Text("Physics");

        float mass = entity.get_mass();
        if (ImGui::DragFloat("Mass", &mass)) {
            entity.set_mass(mass);
        }

        kl::float3 velocity = entity.get_velocity();
        if (ImGui::DragFloat3("Velocity", velocity)) {
            entity.set_velocity(velocity);
        }

        kl::float3 angular = entity.get_angular();
        if (ImGui::DragFloat3("Angular", angular)) {
            entity.set_angular(angular);
        }

        bool gravity = entity.get_gravity();
        if (ImGui::Checkbox("Gravity", &gravity)) {
            entity.set_gravity(gravity);
        }

        // Mesh
        ImGui::Separator();
        ImGui::Text("Mesh");

        std::string bound_mesh = "";
        for (auto& [mesh_name, mesh] : state->meshes) {
            if (mesh == entity.mesh) {
                bound_mesh = mesh_name;
                break;
            }
        }

        if (ImGui::BeginCombo("Bound Mesh", bound_mesh.c_str())) {
            for (auto& [mesh_name, mesh] : state->meshes) {
                if (ImGui::Selectable(mesh_name.c_str(), mesh == entity.mesh)) {
                    entity.mesh = mesh;
                    break;
                }
            }

            ImGui::EndCombo();
        }

        // Material
        ImGui::Separator();
        ImGui::Text("Material");

        std::string bound_material = "";
        for (auto& [material_name, material] : state->materials) {
            if (material == entity.material) {
                bound_material = material_name;
                break;
            }
        }

        if (ImGui::BeginCombo("Bound Material", bound_material.c_str())) {
            for (auto& [material_name, material] : state->materials) {
                if (ImGui::Selectable(material_name.c_str(), material == entity.material)) {
                    entity.material = material;
                    break;
                }
            }

            ImGui::EndCombo();
        }

        // Final
        ImGui::Separator();
    }
    ImGui::End();
}
