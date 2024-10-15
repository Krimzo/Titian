#include "titian.h"


titian::EditorPass::EditorPass()
    : RenderPass("EditorPass")
{
    kl::GPU& gpu = Layers::get<AppLayer>().gpu;
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
    frustum_mesh = gpu.create_vertex_buffer(frustum_data);
}

void titian::EditorPass::state_package(StatePackage& package)
{
    RenderLayer& render_layer = Layers::get<RenderLayer>();
    package.raster_state = render_layer.raster_states.wireframe;
    package.depth_state = render_layer.depth_states.enabled;
    package.shader_state = render_layer.shader_states.solid_pass;
}

void titian::EditorPass::render_self(StatePackage& package)
{
    EditorLayer& editor_layer = Layers::get<EditorLayer>();
    RenderLayer& render_layer = Layers::get<RenderLayer>();
    GUILayer& gui_layer = Layers::get<GUILayer>();

    kl::GPU& gpu = Layers::get<AppLayer>().gpu;
    Scene& scene = Layers::get<GameLayer>().scene();

    if (editor_layer.selected_entities.empty())
        return;

    gpu.bind_target_depth_view(package.camera->screen_texture.target_view, package.camera->depth_texture.depth_view);

    for (auto& name : editor_layer.selected_entities) {
        Entity* entity = scene.helper_get_entity(name);
        if (!entity)
            continue;

        struct alignas(16) CB
        {
            Float4x4 WVP;
            Float4 SOLID_COLOR;
        };

        {
            const auto& cube = scene.default_meshes.cube;
            const auto& sphere = scene.default_meshes.sphere;
			const auto& capsule = scene.default_meshes.capsule;

            CB cb{};
            cb.WVP = package.camera->camera_matrix() * entity->collider_matrix();
            cb.SOLID_COLOR = gui_layer.alternate_color;
            package.shader_state.upload(cb);

            switch (entity->geometry_type())
            {
            case px::PxGeometryType::Enum::eBOX:
                gpu.draw(cube.graphics_buffer, cube.topology, sizeof(Vertex));
                bench_add_draw_call();
                break;

            case px::PxGeometryType::Enum::eSPHERE:
                gpu.draw(sphere.graphics_buffer, sphere.topology, sizeof(Vertex));
                bench_add_draw_call();
                break;

            case px::PxGeometryType::Enum::eCAPSULE:
                gpu.draw(capsule.graphics_buffer, capsule.topology, sizeof(Vertex));
                bench_add_draw_call();
                break;
            }
        }

        if (Camera* camera = dynamic_cast<Camera*>(entity)) {
            CB cb{};
            cb.WVP = package.camera->camera_matrix() * kl::inverse(camera->camera_matrix());
            cb.SOLID_COLOR = gui_layer.special_color;
            package.shader_state.upload(cb);
            gpu.draw(frustum_mesh, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
            bench_add_draw_call();
        }
    }

    gpu.unbind_target_depth_views();
}
