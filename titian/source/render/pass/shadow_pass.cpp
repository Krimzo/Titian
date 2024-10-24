#include "titian.h"


titian::ShadowPass::ShadowPass()
    : RenderPass("ShadowPass")
{}

void titian::ShadowPass::state_package(StatePackage& package)
{
    RenderLayer& render_layer = Layers::get<RenderLayer>();
    package.raster_state = render_layer.raster_states.shadow;
    package.depth_state = render_layer.depth_states.enabled;
    package.shaders = render_layer.shader_states.shadow_pass;
}

void titian::ShadowPass::render_self(StatePackage& package)
{
    RenderLayer& render_layer = Layers::get<RenderLayer>();
    kl::Timer& timer = Layers::get<AppLayer>().timer;
    kl::GPU& gpu = Layers::get<AppLayer>().gpu;
    Scene& scene = Layers::get<GameLayer>().scene();

    DirectionalLight* dir_light = scene.get_casted<DirectionalLight>(scene.main_directional_light_name);
    if (!dir_light)
        return;

    struct alignas(16) CB
    {
        Float4x4 WVP;
        float IS_SKELETAL;
    };

    struct RenderInfo
    {
        Animation* animation = nullptr;
        Mesh* mesh = nullptr;
        CB cb = {};
    };

    Vector<RenderInfo> to_render;
    to_render.reserve(scene.entities().size());
    
    const auto schedule_entity_helper = [&](Entity* entity)
    {
        if (!entity->shadows)
            return;

        RenderInfo info{};
        info.animation = scene.helper_get_animation(entity->animation_name);
        if (!info.animation) {
            return;
        }

        info.mesh = info.animation->get_mesh(scene, timer.elapsed());
        Material* material = scene.helper_get_material(entity->material_name);
        if (!info.mesh || !material || material->is_transparent()) {
            return;
        }

        info.cb.WVP = entity->model_matrix();
        if (info.animation->animation_type == AnimationType::SKELETAL) {
            info.cb.IS_SKELETAL = 1.0f;
        }
        to_render.push_back(info);
    };

    for (const auto& [_, entity] : scene.entities()) {
        schedule_entity_helper(&entity);
    }

    gpu.set_viewport_size(Int2{ dir_light->resolution() });

    bool wireframe_bound = package.camera->render_wireframe;
    gpu.bind_raster_state(wireframe_bound ? render_layer.raster_states.wireframe : render_layer.raster_states.shadow);

    for (int i = 0; i < DirectionalLight::CASCADE_COUNT; i++) {
        const Float4x4 VP = dir_light->light_matrix(package.camera, i);
        const dx::DepthView shadow_map = dir_light->depth_view(i);

        gpu.bind_target_depth_views({}, shadow_map);
        gpu.clear_depth_view(shadow_map, 1.0f, 0xFF);

        for (const auto& info : to_render) {
            const bool should_wireframe = package.camera->render_wireframe || info.mesh->render_wireframe;
            if (should_wireframe != wireframe_bound) {
                wireframe_bound = should_wireframe;
                gpu.bind_raster_state(wireframe_bound ? render_layer.raster_states.wireframe : render_layer.raster_states.shadow);
            }
            if (info.animation->animation_type == AnimationType::SKELETAL) {
                info.animation->bind_matrices(0);
            }

            CB cb = info.cb;
            cb.WVP = VP * cb.WVP;
            package.shaders.upload(cb);
            gpu.draw(info.mesh->buffer, info.mesh->d3d_topology(), sizeof(Vertex));
            bench_add_draw_call();
        }
    }

    gpu.unbind_shader_view_for_vertex_shader(0);
}
