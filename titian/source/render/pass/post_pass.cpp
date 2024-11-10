#include "titian.h"


titian::PostPass::PostPass()
    : RenderPass( "PostPass" )
{}

void titian::PostPass::state_package( StatePackage& package )
{
    RenderLayer& render_layer = RenderLayer::get();
    package.raster_state = render_layer.raster_states.solid;
    package.depth_state = render_layer.depth_states.disabled;
    package.blend_state = render_layer.blend_states.disabled;
    package.shaders = render_layer.shader_states.post_pass;
}

void titian::PostPass::render_self( StatePackage& package )
{
    RenderLayer& render_layer = RenderLayer::get();
    kl::GPU& gpu = AppLayer::get().gpu;
    Scene& scene = GameLayer::get().scene();

    gpu.bind_target_depth_view( package.camera->screen_texture.target_view, {} );

    gpu.bind_shader_view_for_pixel_shader( package.camera->color_texture.shader_view, 0 );
    gpu.bind_shader_view_for_pixel_shader( package.camera->depth_texture.shader_view, 1 );
    gpu.bind_shader_view_for_pixel_shader( package.camera->index_texture.shader_view, 2 );

    gpu.bind_sampler_state_for_pixel_shader( render_layer.sampler_states.linear, 0 );
    gpu.bind_sampler_state_for_pixel_shader( render_layer.sampler_states.non_linear, 1 );

    struct alignas(16) CB
    {
        Float4x4 CUSTOM_DATA;
        Float2 FRAME_SIZE;
    };

    CB cb{};
    cb.FRAME_SIZE = package.camera->resolution();
    cb.CUSTOM_DATA = package.camera->custom_data;

    kl::Shaders* shaders = &package.shaders;
    if ( Shader* shader = scene.helper_get_shader( package.camera->shader_name ) )
        shaders = &shader->shaders;

    if ( *shaders )
    {
        gpu.bind_shaders( *shaders );
        shaders->upload( cb );
        gpu.draw( render_layer.screen_mesh );
        bench_add_draw_call();
    }

    for ( int i = 0; i < 3; i++ )
        gpu.unbind_shader_view_for_pixel_shader( i );

    gpu.unbind_target_depth_views();
}
