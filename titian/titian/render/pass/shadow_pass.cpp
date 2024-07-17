#include "main.h"


titian::ShadowPass::ShadowPass(const LayerPackage& package)
    : RenderPass("ShadowPass", package)
{}

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
    // Helper
    RenderStates* render_states = &render_layer->states;
    kl::Timer* timer = &app_layer->timer;
    kl::GPU* gpu = &app_layer->gpu;
    Scene* scene = &game_layer->scene;

    // Skip if no camera
    Camera* camera = scene->get_casted<Camera>(scene->main_camera_name);
    if (!camera) { return; }

    // Skip if no directional light
    DirectionalLight* dir_light = scene->get_casted<DirectionalLight>(scene->main_directional_light_name);
    if (!dir_light) { return; }

    // Update target view/viewport
    gpu->set_viewport_size(kl::Int2{ (int) dir_light->map_resolution() });

    // Render shadows
    for (int i = 0; i < kl::DirectionalLight::CASCADE_COUNT; i++) {
        // Helper
        const kl::Float4x4 VP = dir_light->light_matrix(camera, i);
        const kl::dx::DepthView shadow_map = dir_light->depth_view(i);

        // Target
        gpu->bind_target_depth_views({}, shadow_map);
        gpu->clear_depth_view(shadow_map, 1.0f, 0xFF);

        // Draw
        for (auto& [_, entity] : *scene) {
            // Skip
            if (!entity->casts_shadows) {
				continue;
			}

            Animation* animation = &scene->get_animation(entity->animation_name);
			if (!animation) {
				continue;
			}

            Mesh* mesh = animation->get_mesh(timer->elapsed());
            Material* material = &scene->get_material(entity->material_name);
            if (!mesh || !material || material->is_transparent()) {
                continue;
            }

            // Bind raster state
            if (!render_layer->render_wireframe) {
                gpu->bind_raster_state(mesh->render_wireframe ? render_states->raster_states->wireframe : render_states->raster_states->shadow);
            }

            // Set cb data
            struct VS_CB
            {
                kl::Float4x4 WVP;
            };

            const VS_CB vs_cb{
                .WVP = VP * entity->model_matrix(),
            };
            package.shader_state.vertex_shader.update_cbuffer(vs_cb);

            // Draw
            gpu->draw(mesh->graphics_buffer, mesh->casted_topology(), sizeof(Vertex));
        }
    }
}
