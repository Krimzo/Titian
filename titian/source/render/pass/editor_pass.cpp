#include "titian.h"


titian::EditorPass::EditorPass()
    : RenderPass( "EditorPass" )
{
    kl::GPU const& gpu = AppLayer::get().gpu;
    kl::Vertex frustum_vertices[8] = {
        { { 1.0f, 1.0f, -1.0f } },
        { { -1.0f, 1.0f, -1.0f } },
        { { 1.0f, -1.0f, -1.0f } },
        { { -1.0f, -1.0f, -1.0f } },
        { { 1.0f, 1.0f, 1.0f } },
        { { -1.0f, 1.0f, 1.0f } },
        { { 1.0f, -1.0f, 1.0f } },
        { { -1.0f, -1.0f, 1.0f } },
    };
    Vector<kl::Vertex> frustum_data = {
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
    frustum_mesh = gpu.create_vertex_buffer( frustum_data );
}

void titian::EditorPass::state_package( StatePackage& package )
{
    RenderLayer& render_layer = RenderLayer::get();
    package.raster_state = render_layer.raster_states.wireframe;
    package.depth_state = render_layer.depth_states.enabled;
    package.shaders = render_layer.shader_states.solid_pass;
}

void titian::EditorPass::render_self( StatePackage& package )
{
    EditorLayer& editor_layer = EditorLayer::get();
    GUILayer& gui_layer = GUILayer::get();

    kl::GPU& gpu = AppLayer::get().gpu;
    Scene& scene = GameLayer::get().scene();

    if ( editor_layer.selected_entities.empty() )
        return;

    gpu.bind_target_depth_view( package.camera->screen_texture.target_view, package.camera->depth_texture.depth_view );

    for ( auto& name : editor_layer.selected_entities )
    {
        Entity* entity = scene.helper_get_entity( name );
        if ( !entity )
            continue;

        struct alignas(16) CB
        {
            Float4x4 WVP;
            Float4 SOLID_COLOR;
        };

        {
            auto& cube = scene.default_meshes.cube;
            auto& sphere = scene.default_meshes.sphere;
            auto& capsule = scene.default_meshes.capsule;

            CB cb{};
            cb.WVP = package.camera->camera_matrix() * entity->collider_matrix();
            cb.SOLID_COLOR = gui_layer.alternate_color;
            package.shaders.upload( cb );

            switch ( entity->geometry_type() )
            {
            case px::PxGeometryType::Enum::eBOX:
                gpu.draw( cube.buffer, cube.d3d_topology(), sizeof( Vertex ) );
                bench_add_draw_call();
                break;

            case px::PxGeometryType::Enum::eSPHERE:
                gpu.draw( sphere.buffer, sphere.d3d_topology(), sizeof( Vertex ) );
                bench_add_draw_call();
                break;

            case px::PxGeometryType::Enum::eCAPSULE:
                gpu.draw( capsule.buffer, capsule.d3d_topology(), sizeof( Vertex ) );
                bench_add_draw_call();
                break;
            }
        }

        if ( Camera* camera = dynamic_cast<Camera*>(entity) )
        {
            CB cb{};
            cb.WVP = package.camera->camera_matrix() * kl::inverse( camera->camera_matrix() );
            cb.SOLID_COLOR = gui_layer.special_color;
            package.shaders.upload( cb );
            gpu.draw( frustum_mesh, D3D_PRIMITIVE_TOPOLOGY_LINELIST );
            bench_add_draw_call();
        }
    }

    gpu.unbind_target_depth_views();
}
