#include "titian.h"


titian::SkyboxPass::SkyboxPass()
    : RenderPass("SkyboxPass")
{}

bool titian::SkyboxPass::is_renderable() const
{
    Scene* scene = &Layers::get<GameLayer>()->scene;
    Camera* camera = scene->get_casted<Camera>(scene->main_camera_name);
    return static_cast<bool>(camera);
}

titian::StatePackage titian::SkyboxPass::get_state_package()
{
    RenderStates* render_states = &Layers::get<RenderLayer>()->states;

    StatePackage package = {};
    package.raster_state = render_states->raster_states->solid;
    package.depth_state = render_states->depth_states->disabled;
    package.shader_state = render_states->shader_states->skybox_pass;
    return package;
}

void titian::SkyboxPass::render_self(StatePackage& package)
{
    // prepare
    RenderLayer* render_layer = Layers::get<RenderLayer>();
    RenderStates* render_states = &render_layer->states;
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
    Scene* scene = &Layers::get<GameLayer>()->scene;

    Camera* camera = scene->get_casted<Camera>(scene->main_camera_name);
    if (!camera)
        return;

    Texture* skybox = scene->helper_get_texture(camera->skybox_name);
    if (!skybox)
        return;

    // render
    struct VS_CB
    {
        Float4x4 VP;
    };

    VS_CB vs_cb{};
    vs_cb.VP = camera->camera_matrix();
    package.shader_state.vertex_shader.update_cbuffer(vs_cb);

    gpu->bind_target_depth_view(render_layer->game_color_texture->target_view, {});
    gpu->bind_sampler_state_for_pixel_shader(render_states->sampler_states->linear, 0);
    gpu->bind_shader_view_for_pixel_shader(skybox->shader_view, 0);
   
    gpu->draw(scene->default_meshes->cube->graphics_buffer, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, sizeof(Vertex));
    bench_add_draw_call();
}
