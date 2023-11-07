#include "main.h"


titian::EditorUnlitPass::EditorUnlitPass(GameLayer* game_layer, EditorLayer* editor_layer, GUILayer* gui_layer)
    : RenderPass(game_layer)
{
    this->editor_layer = editor_layer;
    this->gui_layer = gui_layer;

    kl::GPU* gpu = &game_layer->app_layer->gpu;

    // Generate frustum mesh
    const kl::Vertex frustum_vertices[8] = {
        { {  1.0f,  1.0f, -1.0f } },
        { { -1.0f,  1.0f, -1.0f } },
        { {  1.0f, -1.0f, -1.0f } },
        { { -1.0f, -1.0f, -1.0f } },
        { {  1.0f,  1.0f,  1.0f } },
        { { -1.0f,  1.0f,  1.0f } },
        { {  1.0f, -1.0f,  1.0f } },
        { { -1.0f, -1.0f,  1.0f } },
    };
    const std::vector<kl::Vertex> frustum_data = {
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

bool titian::EditorUnlitPass::is_renderable() const
{
    const Scene* scene = &game_layer->scene;
    const Entity* entity = &scene->get_entity(editor_layer->selected_entity);
    return static_cast<bool>(entity);
}

titian::StatePackage titian::EditorUnlitPass::get_state_package()
{
    RenderStates* render_states = &gui_layer->render_layer->states;

    StatePackage package = {};
    package.raster_state = render_states->raster_states->wireframe;
    package.depth_state = render_states->depth_states->enabled;
    package.shader_state = render_states->shader_states->unlit_pass;
    return package;
}

void titian::EditorUnlitPass::render_self(StatePackage& package)
{
    kl::GPU* gpu = &game_layer->app_layer->gpu;
    Scene* scene = &game_layer->scene;

    Camera* main_camera = scene->get_casted<Camera>(scene->main_camera_name);
    if (!main_camera) {
        return;
    }

    Entity* selected_entity = scene->get_casted<Entity>(editor_layer->selected_entity);
    if (!selected_entity) {
        return;
    }

    // Collider
    if (Collider* collider = &selected_entity->collider()) {
        const auto& default_meshes = scene->default_meshes;

        struct VSData
        {
            kl::Float4x4 wvp_matrix;
        } vs_data = {};
        vs_data.wvp_matrix = main_camera->camera_matrix() * selected_entity->collider_matrix();

        struct PSData
        {
            kl::Float4 object_color; // (color.r, color.g, color.b, none)
        } ps_data = {};
        ps_data.object_color = gui_layer->alternate_color;

        package.shader_state.vertex_shader.update_cbuffer(vs_data);
        package.shader_state.pixel_shader.update_cbuffer(ps_data);

        switch (collider->type()) {
        case physx::PxGeometryType::Enum::eBOX:
            gpu->draw(default_meshes->cube->graphics_buffer, default_meshes->cube->casted_topology());
            break;

        case physx::PxGeometryType::Enum::eSPHERE:
            gpu->draw(default_meshes->sphere->graphics_buffer, default_meshes->sphere->casted_topology());
            break;

        case physx::PxGeometryType::Enum::eCAPSULE:
            gpu->draw(default_meshes->capsule->graphics_buffer, default_meshes->capsule->casted_topology());
            break;

        case physx::PxGeometryType::Enum::eTRIANGLEMESH:
            if (Mesh* mesh = &scene->get_mesh(selected_entity->mesh_name)) {
                gpu->draw(mesh->graphics_buffer, mesh->casted_topology());
            }
            break;
        }
    }

    // Camera frustum
    if (Camera* selected_camera = dynamic_cast<Camera*>(selected_entity)) {
        struct VSData
        {
            kl::Float4x4 wvp_matrix;
        } vs_data = {};
        vs_data.wvp_matrix = main_camera->camera_matrix() * kl::inverse(selected_camera->camera_matrix());

        struct PSData
        {
            kl::Float4 object_color; // (color.r, color.g, color.b, none)
        } ps_data = {};
        ps_data.object_color = kl::colors::WHITE;

        package.shader_state.vertex_shader.update_cbuffer(vs_data);
        package.shader_state.pixel_shader.update_cbuffer(ps_data);

        gpu->draw(frustum_mesh, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    }
}
