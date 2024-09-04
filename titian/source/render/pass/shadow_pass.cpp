#include "titian.h"


titian::ShadowPass::ShadowPass()
    : RenderPass("ShadowPass")
{}

bool titian::ShadowPass::is_renderable() const
{
    Scene* scene = &Layers::get<GameLayer>()->scene;
    DirectionalLight* dir_light = scene->get_casted<DirectionalLight>(scene->main_directional_light_name);
    return static_cast<bool>(dir_light);
}

titian::StatePackage titian::ShadowPass::get_state_package()
{
    RenderStates* render_states = &Layers::get<RenderLayer>()->states;

    StatePackage package = {};
    package.raster_state = render_states->raster_states->shadow;
    package.depth_state = render_states->depth_states->enabled;
    package.shader_state = render_states->shader_states->shadow_pass;
    return package;
}

void titian::ShadowPass::render_self(StatePackage& package)
{
    // prep
    RenderLayer* render_layer = Layers::get<RenderLayer>();
    RenderStates* render_states = &render_layer->states;
    kl::Timer* timer = &Layers::get<AppLayer>()->timer;
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
    Scene* scene = &Layers::get<GameLayer>()->scene;

    Camera* camera = scene->get_casted<Camera>(scene->main_camera_name);
    if (!camera)
        return;

    DirectionalLight* dir_light = scene->get_casted<DirectionalLight>(scene->main_directional_light_name);
    if (!dir_light)
        return;

    // collect
    struct alignas(16) VS_CB
    {
        Float4x4 WVP = {};
        float IS_SKELETAL = 0.0f;
    };

    struct RenderInfo
    {
        Animation* animation = nullptr;
        Mesh* mesh = nullptr;
        VS_CB vs_cb = {};
    };

    Vector<RenderInfo> to_render;
    to_render.reserve(scene->entities().size());

    for (const auto& [_, entity] : scene->entities()) {
        if (!entity->casts_shadows) {
            continue;
        }

        RenderInfo info{};
        info.animation = scene->helper_get_animation(entity->animation_name);
        if (!info.animation) {
            continue;
        }

        info.mesh = info.animation->get_mesh(timer->elapsed());
        Material* material = scene->helper_get_material(entity->material_name);
        if (!info.mesh || !material || material->is_transparent()) {
            continue;
        }

        info.vs_cb.WVP = entity->model_matrix();
        if (info.animation->animation_type == AnimationType::SKELETAL) {
            info.vs_cb.IS_SKELETAL = 1.0f;
        }
        to_render.push_back(info);
    }

    // render
    gpu->set_viewport_size(Int2{ dir_light->resolution() });

    bool wireframe_bound = render_layer->render_wireframe;
    gpu->bind_raster_state(wireframe_bound ? render_states->raster_states->wireframe : render_states->raster_states->shadow);

    for (int i = 0; i < DirectionalLight::CASCADE_COUNT; i++) {
        const Float4x4 VP = dir_light->light_matrix(camera, i);
        const dx::DepthView shadow_map = dir_light->depth_view(i);

        gpu->bind_target_depth_views({}, shadow_map);
        gpu->clear_depth_view(shadow_map, 1.0f, 0xFF);

        for (const auto& info : to_render) {
            const bool should_wireframe = render_layer->render_wireframe || info.mesh->render_wireframe;
            if (should_wireframe != wireframe_bound) {
                wireframe_bound = should_wireframe;
                gpu->bind_raster_state(wireframe_bound ? render_states->raster_states->wireframe : render_states->raster_states->shadow);
            }

            if (info.animation->animation_type == AnimationType::SKELETAL) {
                info.animation->bind_matrices(0);
            }

            VS_CB vs_cb = info.vs_cb;
            vs_cb.WVP = VP * vs_cb.WVP;
            package.shader_state.vertex_shader.update_cbuffer(vs_cb);
            gpu->draw(info.mesh->graphics_buffer, info.mesh->casted_topology(), sizeof(Vertex));
            bench_add_draw_call();
        }
    }

    // finalize
    gpu->unbind_shader_view_for_vertex_shader(0);
}
