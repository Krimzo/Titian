#include "titian.h"


titian::SkyboxPass::SkyboxPass()
    : RenderPass( "SkyboxPass" )
{}

void titian::SkyboxPass::state_package( StatePackage& package )
{
    RenderLayer& render_layer = RenderLayer::get();
    package.raster_state = render_layer.raster_states.solid;
    package.depth_state = render_layer.depth_states.disabled;
    package.shaders = render_layer.shader_states.skybox_pass;
}

void titian::SkyboxPass::render_self( StatePackage& package )
{
    RenderLayer& render_layer = RenderLayer::get();
    kl::GPU& gpu = AppLayer::get().gpu;
    Scene& scene = GameLayer::get().scene();

    Texture* skybox = scene.helper_get_texture( package.camera->skybox_texture_name );
    if ( !skybox )
        return;

    struct alignas(16) CB
    {
        Float4x4 VP;
    } cb = {};

    cb.VP = package.camera->camera_matrix();
    package.shaders.upload( cb );

    gpu.bind_target_depth_view( package.camera->color_texture.target_view, {} );
    gpu.bind_sampler_state_for_pixel_shader( render_layer.sampler_states.linear, 0 );
    gpu.bind_shader_view_for_pixel_shader( skybox->shader_view, 0 );

    gpu.draw( scene.default_meshes.cube.buffer, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, sizeof( Vertex ) );
    bench_add_draw_call();

    gpu.unbind_target_depth_views();
}
