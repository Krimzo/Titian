#include "titian.h"


titian::EditorPass::EditorPass(const LayerPackage& package)
    : RenderPass("EditorPass", package)
{
    kl::GPU* gpu = &app_layer->gpu;
    const kl::Vertex<float> frustum_vertices[8] = {
        { {  1.0f,  1.0f, -1.0f } },
        { { -1.0f,  1.0f, -1.0f } },
        { {  1.0f, -1.0f, -1.0f } },
        { { -1.0f, -1.0f, -1.0f } },
        { {  1.0f,  1.0f,  1.0f } },
        { { -1.0f,  1.0f,  1.0f } },
        { {  1.0f, -1.0f,  1.0f } },
        { { -1.0f, -1.0f,  1.0f } },
    };
    const Vector<kl::Vertex<float>> frustum_data = {
        frustum_vertices[0], frustum_vertices[1],
        frustum_vertices[0], frustum_vertices[2],
        frustum_vertices[3], frustum_vertices[1],
        frustum_vertices[3], frustum_vertices[2],
        frustum_vertices[4], frustum_vertices[5],
        frustum_vertices[4], frustum_vertices[6],
        frustum_vertices[7], frustum_vertices[5],
        frustum_vertices[7], frustum_vertices[6],
        frustum_vertices[0], frustum_vertices[4],
        frustum_vertices[1], frustum_vertices[5],
        frustum_vertices[2], frustum_vertices[6],
        frustum_vertices[3], frustum_vertices[7],
    };
    frustum_mesh = gpu->create_vertex_buffer(frustum_data);
}

bool titian::EditorPass::is_renderable() const
{
    return !editor_layer->selected_entities.empty();
}

titian::StatePackage titian::EditorPass::get_state_package()
{
    RenderStates* render_states = &render_layer->states;

    StatePackage package = {};
    package.raster_state = render_states->raster_states->wireframe;
    package.depth_state = render_states->depth_states->enabled;
    package.shader_state = render_states->shader_states->solid_pass;
    return package;
}

void titian::EditorPass::render_self(StatePackage& package)
{
    kl::GPU* gpu = &app_layer->gpu;
    Scene* scene = &game_layer->scene;

    Camera* main_camera = scene->get_casted<Camera>(scene->main_camera_name);
    if (!main_camera) {
        return;
    }

    // Target
    gpu->bind_target_depth_view(render_layer->screen_texture->target_view, render_layer->game_depth_texture->depth_view);

    for (auto& sel_name : editor_layer->selected_entities) {
        Entity* selected_entity = scene->get_casted<Entity>(sel_name);
        if (!selected_entity)
            continue;

        struct VS_CB
        {
            Float4x4 WVP;
        };
        struct PS_CB
        {
            Float4 SOLID_COLOR;
        };

        // Collider
        if (Collider* collider = &selected_entity->collider()) {
            const auto& default_meshes = scene->default_meshes;

            const VS_CB vs_cb{
                .WVP = main_camera->camera_matrix() * selected_entity->collider_matrix(),
            };
            const PS_CB ps_cb{
                .SOLID_COLOR = gui_layer->alternate_color,
            };
            package.shader_state.vertex_shader.update_cbuffer(vs_cb);
            package.shader_state.pixel_shader.update_cbuffer(ps_cb);

            switch (collider->type()) {
            case px::PxGeometryType::Enum::eBOX:
                gpu->draw(default_meshes->cube->graphics_buffer, default_meshes->cube->casted_topology(), sizeof(Vertex));
                break;

            case px::PxGeometryType::Enum::eSPHERE:
                gpu->draw(default_meshes->sphere->graphics_buffer, default_meshes->sphere->casted_topology(), sizeof(Vertex));
                break;

            case px::PxGeometryType::Enum::eCAPSULE:
                gpu->draw(default_meshes->capsule->graphics_buffer, default_meshes->capsule->casted_topology(), sizeof(Vertex));
                break;

            case px::PxGeometryType::Enum::eTRIANGLEMESH:
                if (Mesh* mesh = &scene->get_mesh(selected_entity->collider_mesh_name)) {
                    gpu->draw(mesh->graphics_buffer, mesh->casted_topology(), sizeof(Vertex));
                }
                break;
            }
        }

        // Camera frustum
        if (Camera* selected_camera = dynamic_cast<Camera*>(selected_entity)) {
            const VS_CB vs_cb{
                .WVP = main_camera->camera_matrix() * kl::inverse(selected_camera->camera_matrix()),
            };
            const PS_CB ps_cb{
                .SOLID_COLOR = kl::colors::WHITE,
            };
            package.shader_state.vertex_shader.update_cbuffer(vs_cb);
            package.shader_state.pixel_shader.update_cbuffer(ps_cb);

            gpu->draw(frustum_mesh, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
        }
    }
}
