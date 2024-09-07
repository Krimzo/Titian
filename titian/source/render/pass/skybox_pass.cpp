#include "titian.h"


titian::SkyboxPass::SkyboxPass()
    : RenderPass("SkyboxPass")
{}

void titian::SkyboxPass::state_package(StatePackage* package)
{
    RenderLayer* render_layer = Layers::get<RenderLayer>();
    package->raster_state = render_layer->raster_states->solid;
    package->depth_state = render_layer->depth_states->disabled;
    package->shader_state = render_layer->shader_states->skybox_pass;
}

void titian::SkyboxPass::render_self(StatePackage* package)
{
    // prepare
    RenderLayer* render_layer = Layers::get<RenderLayer>();
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
    Scene* scene = &Layers::get<GameLayer>()->scene;

    Texture* skybox = scene->helper_get_texture(package->camera->skybox_name);
    if (!skybox)
        return;

    // render
    struct VS_CB
    {
        Float4x4 VP;
    };

    VS_CB vs_cb{};
    vs_cb.VP = package->camera->camera_matrix();
    package->shader_state.vertex_shader.update_cbuffer(vs_cb);

    gpu->bind_target_depth_view(package->camera->game_color_texture->target_view, {});
    gpu->bind_sampler_state_for_pixel_shader(render_layer->sampler_states->linear, 0);
    gpu->bind_shader_view_for_pixel_shader(skybox->shader_view, 0);
   
    gpu->draw(scene->default_meshes->cube->graphics_buffer, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, sizeof(Vertex));
    bench_add_draw_call();
}
