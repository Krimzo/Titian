#include "main.h"


titian::SkyboxPass::SkyboxPass(const LayerPackage& package)
    : RenderPass("SkyboxPass", package)
{}

bool titian::SkyboxPass::is_renderable() const
{
    const Scene* scene = &game_layer->scene;
    const Camera* camera = scene->get_casted<Camera>(scene->main_camera_name);
    return static_cast<bool>(camera);
}

titian::StatePackage titian::SkyboxPass::get_state_package()
{
    RenderStates* render_states = &render_layer->states;

    StatePackage package = {};
    package.raster_state = render_states->raster_states->solid;
    package.depth_state = render_states->depth_states->disabled;
    package.shader_state = render_states->shader_states->skybox_pass;
    return package;
}

void titian::SkyboxPass::render_self(StatePackage& package)
{
    // Helper
    RenderStates* render_states = &render_layer->states;
    kl::GPU* gpu = &app_layer->gpu;
    Scene* scene = &game_layer->scene;

    Camera* camera = scene->get_casted<Camera>(scene->main_camera_name);
    if (!camera)
        return;

    Texture* skybox = &scene->get_texture(camera->skybox_name);
    if (!skybox)
        return;

    // Target
    gpu->bind_target_depth_view(render_layer->render_texture->target_view, render_layer->depth_texture->depth_view);

    // Set cb data
    struct VS_CB
    {
        kl::Float4x4 VP;
    };

    const VS_CB vs_cb{
        .VP = camera->camera_matrix(),
    };
    package.shader_state.vertex_shader.update_cbuffer(vs_cb);

    // Draw
    gpu->bind_sampler_state_for_pixel_shader(render_states->sampler_states->linear, 0);
    gpu->bind_shader_view_for_pixel_shader(skybox->shader_view, 0);
    gpu->draw(scene->default_meshes->cube->graphics_buffer);
}
