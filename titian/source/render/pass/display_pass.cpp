#include "titian.h"


titian::DisplayPass::DisplayPass()
    : RenderPass( "DisplayPass" )
{}

void titian::DisplayPass::state_package( StatePackage& package )
{
    RenderLayer& render_layer = RenderLayer::get();
    package.raster_state = render_layer.raster_states.solid;
    package.depth_state = render_layer.depth_states.disabled;
    package.shaders = render_layer.shader_states.display_pass;
}

void titian::DisplayPass::render_self( StatePackage& package )
{
    RenderLayer& render_layer = RenderLayer::get();
    kl::Window& window = AppLayer::get().window;
    kl::GPU& gpu = AppLayer::get().gpu;

    Int2 window_size = window.size();
    package.camera->resize( window_size );

    gpu.bind_internal_views();
    gpu.set_viewport_size( window_size );
    gpu.bind_shader_view_for_pixel_shader( package.camera->screen_texture.shader_view, 0 );

    gpu.draw( render_layer.screen_mesh );
    bench_add_draw_call();

    gpu.unbind_shader_view_for_pixel_shader( 0 );
    gpu.unbind_target_depth_views();
}
