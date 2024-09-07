#include "titian.h"


titian::PostPass::PostPass()
	: RenderPass("PostPass")
{}

void titian::PostPass::state_package(StatePackage* package)
{
    RenderLayer* render_layer = Layers::get<RenderLayer>();
    package->raster_state = render_layer->raster_states->solid;
    package->depth_state = render_layer->depth_states->disabled;
    package->shader_state = render_layer->shader_states->post_pass;
    package->blend_state = render_layer->blend_states->disabled;
}

void titian::PostPass::render_self(StatePackage* package)
{
    // prepare
    RenderLayer* render_layer = Layers::get<RenderLayer>();
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
    Scene* scene = &Layers::get<GameLayer>()->scene;

    Float4x4 custom_data = package->camera->custom_data;
    if (Shader* shader = scene->helper_get_shader(package->camera->shader_name)) {
        if (shader->graphics_buffer) {
            gpu->bind_render_shaders(shader->graphics_buffer);
        }
    }

    // render
    struct PS_CB
    {
        Float2 FRAME_SIZE;
        alignas(16) Float4x4 CUSTOM_DATA;
    };

	PS_CB ps_cb{};
    ps_cb.FRAME_SIZE = (Float2) package->camera->resolution();
    ps_cb.CUSTOM_DATA = custom_data;
	package->shader_state.pixel_shader.update_cbuffer(ps_cb);
    
    gpu->bind_target_depth_view(package->camera->screen_texture->target_view, {});

    gpu->bind_shader_view_for_pixel_shader(package->camera->game_color_texture->shader_view, 0);
    gpu->bind_shader_view_for_pixel_shader(package->camera->game_depth_texture->shader_view, 1);
    gpu->bind_shader_view_for_pixel_shader(package->camera->editor_picking_texture->shader_view, 2);

    gpu->bind_sampler_state_for_pixel_shader(render_layer->sampler_states->linear, 0);
    gpu->bind_sampler_state_for_pixel_shader(render_layer->sampler_states->non_linear, 1);

	gpu->draw(render_layer->screen_mesh);
    bench_add_draw_call();

    // finalize
    for (int i = 0; i < 3; i++) {
        gpu->unbind_shader_view_for_pixel_shader(i);
    }
    gpu->unbind_target_depth_views();
}
