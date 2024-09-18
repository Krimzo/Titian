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
    RenderLayer* render_layer = Layers::get<RenderLayer>();
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
    Scene* scene = &Layers::get<GameLayer>()->scene;

    Float4x4 custom_data = package->camera->custom_data;
    if (Shader* shader = scene->helper_get_shader(package->camera->shader_name)) {
        if (shader->graphics_buffer) {
            gpu->bind_render_shaders(shader->graphics_buffer);
        }
    }

    struct alignas(16) CB
    {
        Float4x4 CUSTOM_DATA;
        Float2 FRAME_SIZE;
    };

	CB cb{};
    cb.FRAME_SIZE = package->camera->resolution();
    cb.CUSTOM_DATA = custom_data;
	package->shader_state.upload(cb);
    
    gpu->bind_target_depth_view(package->camera->screen_texture->target_view, {});

    gpu->bind_shader_view_for_pixel_shader(package->camera->color_texture->shader_view, 0);
    gpu->bind_shader_view_for_pixel_shader(package->camera->depth_texture->shader_view, 1);
    gpu->bind_shader_view_for_pixel_shader(package->camera->index_texture->shader_view, 2);

    gpu->bind_sampler_state_for_pixel_shader(render_layer->sampler_states->linear, 0);
    gpu->bind_sampler_state_for_pixel_shader(render_layer->sampler_states->non_linear, 1);

	gpu->draw(render_layer->screen_mesh);
    bench_add_draw_call();

    for (int i = 0; i < 3; i++) {
        gpu->unbind_shader_view_for_pixel_shader(i);
    }
    gpu->unbind_target_depth_views();
}
