#include "titian.h"


titian::EditorPass::EditorPass()
    : RenderPass("EditorPass")
{
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
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
    return !Layers::get<EditorLayer>()->selected_entities.empty();
}

titian::StatePackage titian::EditorPass::get_state_package()
{
    RenderStates* render_states = &Layers::get<RenderLayer>()->states;

    StatePackage package = {};
    package.raster_state = render_states->raster_states->wireframe;
    package.depth_state = render_states->depth_states->enabled;
    package.shader_state = render_states->shader_states->solid_pass;
    return package;
}

void titian::EditorPass::render_self(StatePackage& package)
{
    // preapre
    EditorLayer* editor_layer = Layers::get<EditorLayer>();
    RenderLayer* render_layer = Layers::get<RenderLayer>();
    GUILayer* gui_layer = Layers::get<GUILayer>();
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
    Scene* scene = &Layers::get<GameLayer>()->scene;

    Camera* main_camera = scene->get_casted<Camera>(scene->main_camera_name);
    if (!main_camera) {
        return;
    }

    // render
    gpu->bind_target_depth_view(render_layer->screen_texture->target_view, render_layer->game_depth_texture->depth_view);

    for (auto& name : editor_layer->selected_entities) {
        Entity* entity = scene->helper_get_entity(name);
        if (!entity)
            continue;

        struct VS_CB
        {
            Float4x4 WVP;
        };

        struct PS_CB
        {
            Float4 SOLID_COLOR;
        };

        // collider
        if (Collider* collider = &entity->collider()) {
            const auto& cube = scene->default_meshes->cube;
            const auto& sphere = scene->default_meshes->sphere;
			const auto& capsule = scene->default_meshes->capsule;

            const VS_CB vs_cb{
                .WVP = main_camera->camera_matrix() * entity->collider_matrix(),
            };
            const PS_CB ps_cb{
                .SOLID_COLOR = gui_layer->alternate_color,
            };
            package.shader_state.vertex_shader.update_cbuffer(vs_cb);
            package.shader_state.pixel_shader.update_cbuffer(ps_cb);

            switch (collider->type())
            {
            case px::PxGeometryType::Enum::eBOX:
                gpu->draw(cube->graphics_buffer, cube->casted_topology(), sizeof(Vertex));
                bench_add_draw_call();
                break;

            case px::PxGeometryType::Enum::eSPHERE:
                gpu->draw(sphere->graphics_buffer, sphere->casted_topology(), sizeof(Vertex));
                bench_add_draw_call();
                break;

            case px::PxGeometryType::Enum::eCAPSULE:
                gpu->draw(capsule->graphics_buffer, capsule->casted_topology(), sizeof(Vertex));
                bench_add_draw_call();
                break;

            case px::PxGeometryType::Enum::eTRIANGLEMESH:
                if (Mesh* mesh = scene->helper_get_mesh(entity->collider_mesh_name)) {
                    gpu->draw(mesh->graphics_buffer, mesh->casted_topology(), sizeof(Vertex));
                    bench_add_draw_call();
                }
                break;
            }
        }

        // camera frustum
        if (Camera* camera = dynamic_cast<Camera*>(entity)) {
            const VS_CB vs_cb{
                .WVP = main_camera->camera_matrix() * kl::inverse(camera->camera_matrix()),
            };
            const PS_CB ps_cb{
                .SOLID_COLOR = gui_layer->special_color,
            };
            package.shader_state.vertex_shader.update_cbuffer(vs_cb);
            package.shader_state.pixel_shader.update_cbuffer(ps_cb);

            gpu->draw(frustum_mesh, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
            bench_add_draw_call();
        }
    }
}
