#include "main.h"


titian::ShadowPass::ShadowPass(GameLayer* game_layer, RenderLayer* render_layer)
    : RenderPass(game_layer)
{
    this->render_layer = render_layer;
}

bool titian::ShadowPass::is_renderable() const
{
    const Scene* scene = &game_layer->scene;
    const DirectionalLight* dir_light = scene->get_casted<DirectionalLight>(scene->main_directional_light_name);
    return static_cast<bool>(dir_light);
}

titian::StatePackage titian::ShadowPass::get_state_package()
{
    RenderStates* render_states = &render_layer->states;

    StatePackage package = {};
    package.raster_state = render_states->raster_states->shadow;
    package.depth_state = render_states->depth_states->enabled;
    package.shader_state = render_states->shader_states->shadow_pass;
    return package;
}

void titian::ShadowPass::render_self(StatePackage& package)
{
    kl::GPU* gpu = &game_layer->app_layer->gpu;
    Scene* scene = &game_layer->scene;

    Camera* camera = scene->get_casted<Camera>(scene->main_camera_name);
    if (!camera) { return; }

    DirectionalLight* dir_light = scene->get_casted<DirectionalLight>(scene->main_directional_light_name);
    if (!dir_light) { return; }

    gpu->set_viewport_size(kl::Int2{ (int)dir_light->map_resolution() });
    for (int i = 0; i < kl::DirectionalLight::CASCADE_COUNT; i++) {
        const kl::Float4x4 VP = dir_light->light_matrix(camera, i);
        const kl::dx::DepthView shadow_map = dir_light->depth_view(i);

        gpu->bind_target_depth_views({}, shadow_map);
        gpu->clear_depth_view(shadow_map, 1.0f, 0xFF);

        for (auto& [_, entity] : *scene) {
            Mesh* mesh = &scene->get_mesh(entity->mesh_name);
            if (!mesh) { continue; }

            struct VS_CB
            {
                kl::Float4x4 WVP;
            };

            const VS_CB vs_cb{
                .WVP = VP * entity->model_matrix(),
            };
            package.shader_state.vertex_shader.update_cbuffer(vs_cb);

            gpu->draw(mesh->graphics_buffer, mesh->casted_topology());
        }
    }
}
