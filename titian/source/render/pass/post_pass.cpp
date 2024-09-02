#include "titian.h"


titian::PostPass::PostPass()
	: RenderPass("PostPass")
{}

bool titian::PostPass::is_renderable() const
{
	return true;
}

titian::StatePackage titian::PostPass::get_state_package()
{
    RenderStates* render_states = &Layers::get<RenderLayer>()->states;

    StatePackage package{};
    package.raster_state = render_states->raster_states->solid;
    package.depth_state = render_states->depth_states->disabled;
    package.shader_state = render_states->shader_states->post_pass;
    package.blend_state = render_states->blend_states->disabled;
    return package;
}

void titian::PostPass::render_self(StatePackage& package)
{
    // prepare
    RenderLayer* render_layer = Layers::get<RenderLayer>();
    RenderStates* render_states = &render_layer->states;
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
    Scene* scene = &Layers::get<GameLayer>()->scene;

    Float4x4 custom_data;
    if (Camera* camera = scene->get_casted<Camera>(scene->main_camera_name)) {
        custom_data = camera->custom_data;
        if (Shader* shader = scene->helper_get_shader(camera->shader_name)) {
            if (shader->graphics_buffer) {
                gpu->bind_render_shaders(shader->graphics_buffer);
            }
        }
    }

    // render
    struct PS_CB
    {
        Float2 FRAME_SIZE;
        alignas(16) Float4x4 CUSTOM_DATA;
    };

	PS_CB ps_cb{};
    ps_cb.FRAME_SIZE = (Float2) render_layer->get_render_texture_size();
    ps_cb.CUSTOM_DATA = custom_data;
	package.shader_state.pixel_shader.update_cbuffer(ps_cb);
    
    gpu->bind_target_depth_view(render_layer->screen_texture->target_view, {});

    gpu->bind_shader_view_for_pixel_shader(render_layer->game_color_texture->shader_view, 0);
    gpu->bind_shader_view_for_pixel_shader(render_layer->game_depth_texture->shader_view, 1);
    gpu->bind_shader_view_for_pixel_shader(render_layer->editor_picking_texture->shader_view, 2);

    gpu->bind_sampler_state_for_pixel_shader(render_states->sampler_states->linear, 0);
    gpu->bind_sampler_state_for_pixel_shader(render_states->sampler_states->non_linear, 1);

	gpu->draw(render_layer->screen_mesh);

    // finalize
    for (int i = 0; i < 3; i++) {
        gpu->unbind_shader_view_for_pixel_shader(i);
    }
}
