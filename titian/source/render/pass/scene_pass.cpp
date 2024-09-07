#include "titian.h"


titian::ScenePass::ScenePass()
    : RenderPass("ScenePass")
{}

void titian::ScenePass::state_package(StatePackage* package)
{
    RenderLayer* render_layer = Layers::get<RenderLayer>();
    package->raster_state = package->camera->render_wireframe ? render_layer->raster_states->wireframe : dx::RasterState{};
    package->depth_state = render_layer->depth_states->enabled;
    package->shader_state = render_layer->shader_states->scene_pass;
    package->blend_state = render_layer->blend_states->enabled;
}

void titian::ScenePass::render_self(StatePackage* package)
{
    // prepare
	RenderLayer* render_layer = Layers::get<RenderLayer>();
    kl::Timer* timer = &Layers::get<AppLayer>()->timer;
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
    Scene* scene = &Layers::get<GameLayer>()->scene;

    struct GLOBAL_CB
    {
        float ELAPSED_TIME{};
        float DELTA_TIME{};

        alignas(16) Float3 CAMERA_POSITION;
        float CAMERA_HAS_SKYBOX{};
        Float4 CAMERA_BACKGROUND;

        Float3 AMBIENT_COLOR;
        float AMBIENT_INTENSITY{};

        Float3 SUN_DIRECTION;
        float SUN_POINT_SIZE{};
        Float3 SUN_COLOR;

        float OBJECT_INDEX{};
        Float3 OBJECT_SCALE;
        alignas(16) Float3 OBJECT_ROTATION;
        alignas(16) Float3 OBJECT_POSITION;

        alignas(16) Float4 OBJECT_COLOR;
        float TEXTURE_BLEND{};

        float REFLECTION_FACTOR{};
        float REFRACTION_FACTOR{};
        float REFRACTION_INDEX{};

        float HAS_NORMAL_MAP{};
        float HAS_ROUGHNESS_MAP{};

        alignas(16) Float4x4 W;
        Float4x4 V;
        Float4x4 VP;

        float IS_SKELETAL{};

        float RECEIVES_SHADOWS{};
        Float2 SHADOW_MAP_SIZE;
        Float2 SHADOW_MAP_TEXEL_SIZE;
        alignas(16) Float4 SHADOW_CASCADES;
        Float4x4 LIGHT_VPs[DirectionalLight::CASCADE_COUNT];

        Float4x4 CUSTOM_DATA;
    };

    GLOBAL_CB global_cb{};

    global_cb.V = package->camera->view_matrix();
    global_cb.VP = package->camera->camera_matrix();
    global_cb.CAMERA_POSITION = package->camera->position();
    global_cb.CAMERA_HAS_SKYBOX = (float) scene->textures.contains(package->camera->skybox_name);
    global_cb.CAMERA_BACKGROUND = package->camera->background;

    if (AmbientLight* ambient_light = scene->get_casted<AmbientLight>(scene->main_ambient_light_name)) {
        global_cb.AMBIENT_COLOR = ambient_light->color;
        global_cb.AMBIENT_INTENSITY = ambient_light->intensity;
    }
    if (DirectionalLight* directional_light = scene->get_casted<DirectionalLight>(scene->main_directional_light_name)) {
        ID3D11ShaderResourceView* dir_light_views[DirectionalLight::CASCADE_COUNT] = {};
        for (int i = 0; i < DirectionalLight::CASCADE_COUNT; i++) {
            dir_light_views[i] = directional_light->shader_view(i).get();
            global_cb.LIGHT_VPs[i] = directional_light->light_matrix(package->camera, i);
        }

        gpu->context()->PSSetShaderResources(1, DirectionalLight::CASCADE_COUNT, dir_light_views);

        global_cb.SUN_DIRECTION = directional_light->direction();
        global_cb.SUN_COLOR = directional_light->color;
        global_cb.SUN_POINT_SIZE = directional_light->point_size;

        global_cb.SHADOW_MAP_SIZE = Float2{ (float) directional_light->resolution() };
        global_cb.SHADOW_MAP_TEXEL_SIZE = Float2{ 1.0f / directional_light->resolution() };
        for (int i = 0; i < DirectionalLight::CASCADE_COUNT; i++) {
            global_cb.SHADOW_CASCADES[i] = kl::lerp(directional_light->cascade_splits[i + 1], package->camera->near_plane, package->camera->far_plane);
        }
    }
    global_cb.RECEIVES_SHADOWS = true;

    global_cb.ELAPSED_TIME = timer->elapsed();
    global_cb.DELTA_TIME = timer->delta();

    gpu->bind_target_depth_views({ package->camera->game_color_texture->target_view.get(), package->camera->editor_picking_texture->target_view.get() }, package->camera->game_depth_texture->depth_view);
    gpu->bind_sampler_state_for_pixel_shader(render_layer->sampler_states->linear, 0);
    gpu->bind_sampler_state_for_pixel_shader(render_layer->sampler_states->shadow, 1);
    gpu->bind_sampler_state_for_pixel_shader(render_layer->sampler_states->linear, 2);
    if (Texture* skybox = scene->helper_get_texture(package->camera->skybox_name)) {
        gpu->bind_shader_view_for_pixel_shader(skybox->shader_view, 0);
    }
    else {
        gpu->unbind_shader_view_for_pixel_shader(0);
    }

    // collect
    struct RenderInfo
    {
        int id = 0;
        Entity* entity = nullptr;
        Animation* animation = nullptr;
		Mesh* mesh = nullptr;
		Material* material = nullptr;
        const Texture* color_map = nullptr;
		const Texture* normal_map = nullptr;
		const Texture* roughness_map = nullptr;
        kl::RenderShaders* render_shaders = nullptr;
        float camera_distance = 0.0f;
    };

    Vector<RenderInfo> to_render;
    to_render.reserve(scene->entities().size());

    for (int counter = 0; const auto & [_, entity] : scene->entities()) {
        counter += 1;

        if (!package->camera->can_see(entity->position())) {
            continue;
		}

        RenderInfo info{};
        info.id = counter;
        info.entity = &entity;

        info.animation = scene->helper_get_animation(entity->animation_name);
        if (!info.animation) {
            continue;
        }

        info.mesh = info.animation->get_mesh(timer->elapsed());
        info.material = scene->helper_get_material(entity->material_name);
        if (!info.mesh || !info.material) {
            continue;
        }

        info.color_map = scene->helper_get_texture(info.material->color_map_name);
        info.normal_map = scene->helper_get_texture(info.material->normal_map_name);
        info.roughness_map = scene->helper_get_texture(info.material->roughness_map_name);

        if (Shader* shader = scene->helper_get_shader(info.material->shader_name)) {
            info.render_shaders = &shader->graphics_buffer;
        }
        else {
            info.render_shaders = &package->shader_state;
        }

        info.camera_distance = (entity->position() - package->camera->position()).length();
		to_render.push_back(info);
    }

	std::sort(to_render.begin(), to_render.end(), [](const RenderInfo& a, const RenderInfo& b)
    {
        return a.camera_distance < b.camera_distance;
    });
    
    // render helpers
    bool wireframe_bound = package->camera->render_wireframe;
    gpu->bind_raster_state(wireframe_bound ? render_layer->raster_states->wireframe : render_layer->raster_states->solid_cull);

    const auto render_helper = [&](const RenderInfo& info)
    {
        const bool should_wireframe = package->camera->render_wireframe || info.mesh->render_wireframe;
        if (should_wireframe != wireframe_bound) {
            wireframe_bound = should_wireframe;
            gpu->bind_raster_state(wireframe_bound ? render_layer->raster_states->wireframe : render_layer->raster_states->solid_cull);
        }

        if (info.color_map) {
            gpu->bind_shader_view_for_pixel_shader(info.color_map->shader_view, 5);
        }
        else {
            gpu->unbind_shader_view_for_pixel_shader(5);
        }
        if (info.normal_map) {
            gpu->bind_shader_view_for_pixel_shader(info.normal_map->shader_view, 6);
            global_cb.HAS_NORMAL_MAP = 1.0f;
        }
        else {
            global_cb.HAS_NORMAL_MAP = 0.0f;
        }
        if (info.roughness_map) {
            gpu->bind_shader_view_for_pixel_shader(info.roughness_map->shader_view, 7);
            global_cb.HAS_ROUGHNESS_MAP = 1.0f;
        }
        else {
            global_cb.HAS_ROUGHNESS_MAP = 0.0f;
        }

        global_cb.W = info.entity->model_matrix();

        global_cb.OBJECT_INDEX = static_cast<float>(info.id);
        global_cb.OBJECT_SCALE = info.entity->scale;
        global_cb.OBJECT_ROTATION = info.entity->rotation();
        global_cb.OBJECT_POSITION = info.entity->position();

        global_cb.OBJECT_COLOR = info.material->color;
        global_cb.TEXTURE_BLEND = info.material->texture_blend;

        global_cb.REFLECTION_FACTOR = info.material->reflection_factor;
        global_cb.REFRACTION_FACTOR = info.material->refraction_factor;
        global_cb.REFRACTION_INDEX = info.material->refraction_index;

        if (info.animation->animation_type == AnimationType::SKELETAL) {
            info.animation->bind_matrices(0);
            global_cb.IS_SKELETAL = 1.0f;
        }
        else {
            global_cb.IS_SKELETAL = 0.0f;
        }

        global_cb.CUSTOM_DATA = info.material->custom_data;

        if (*info.render_shaders) {
            info.render_shaders->vertex_shader.update_cbuffer(global_cb);
            info.render_shaders->pixel_shader.update_cbuffer(global_cb);
            gpu->bind_render_shaders(*info.render_shaders);
            gpu->draw(info.mesh->graphics_buffer, info.mesh->casted_topology(), sizeof(Vertex));
            bench_add_draw_call();
        }
    };

    // render opaque
    for (const auto& info : to_render) {
        if (!info.material->is_transparent()) {
            render_helper(info);
        }
    }

    // render transparent
    gpu->bind_depth_state(render_layer->depth_states->only_compare);
    global_cb.RECEIVES_SHADOWS = false;

    for (int i = (int) to_render.size() - 1; i >= 0; i--) {
        const auto& info = to_render[i];
        if (info.material->is_transparent()) {
            render_helper(info);
        }
    }

    // finalize
    for (int i = 0; i < 8; i++) {
        gpu->unbind_shader_view_for_pixel_shader(i);
    }
}
