#include "titian.h"


titian::ScenePass::ScenePass()
    : RenderPass("ScenePass")
{}

void titian::ScenePass::state_package(StatePackage& package)
{
    RenderLayer& render_layer = Layers::get<RenderLayer>();
    package.raster_state = package.camera->render_wireframe ? render_layer.raster_states.wireframe : dx::RasterState{};
    package.depth_state = render_layer.depth_states.enabled;
    package.shader_state = render_layer.shader_states.scene_pass;
    package.blend_state = render_layer.blend_states.enabled;
}

void titian::ScenePass::render_self(StatePackage& package)
{
	RenderLayer& render_layer = Layers::get<RenderLayer>();
    kl::Timer& timer = Layers::get<AppLayer>().timer;
    kl::GPU& gpu = Layers::get<AppLayer>().gpu;
    Scene& scene = Layers::get<GameLayer>().scene();

    struct alignas(16) CB
    {
        Float4x4 W;
        Float4x4 V;
        Float4x4 VP;
        Float4x4 LIGHT_VPs[DirectionalLight::CASCADE_COUNT];
        Float4x4 CUSTOM_DATA;
        Float4 CAMERA_BACKGROUND;
        Float4 MATERIAL_COLOR;
        Float4 SHADOW_CASCADES;
        Float3 CAMERA_POSITION;
        float CAMERA_HAS_SKYBOX;
        Float3 AMBIENT_COLOR;
        float RECEIVES_SHADOWS;
        Float3 SUN_DIRECTION;
        float SUN_POINT_SIZE;
        Float3 SUN_COLOR;
        float ELAPSED_TIME;
        Float3 OBJECT_SCALE;
        float DELTA_TIME;
        Float3 OBJECT_ROTATION;
        float OBJECT_INDEX;
        Float3 OBJECT_POSITION;
        float TEXTURE_BLEND;
        Float2 SHADOW_TEXTURE_SIZE;
        Float2 SHADOW_TEXTURE_TEXEL_SIZE;
        float REFLECTIVITY_FACTOR;
        float REFRACTION_INDEX;
        float HAS_NORMAL_TEXTURE;
        float HAS_ROUGHNESS_TEXTURE;
        float IS_SKELETAL;
    } cb = {};

    cb.V = package.camera->view_matrix();
    cb.VP = package.camera->camera_matrix();
    cb.CAMERA_POSITION = package.camera->position();
    cb.CAMERA_HAS_SKYBOX = (float) scene.textures.contains(package.camera->skybox_texture_name);
    cb.CAMERA_BACKGROUND = package.camera->background;

    if (AmbientLight* ambient_light = scene.get_casted<AmbientLight>(scene.main_ambient_light_name)) {
        cb.AMBIENT_COLOR = ambient_light->color;
    }
    if (DirectionalLight* directional_light = scene.get_casted<DirectionalLight>(scene.main_directional_light_name)) {
        ID3D11ShaderResourceView* dir_light_views[DirectionalLight::CASCADE_COUNT] = {};
        for (int i = 0; i < DirectionalLight::CASCADE_COUNT; i++) {
            dir_light_views[i] = directional_light->shader_view(i).get();
            cb.LIGHT_VPs[i] = directional_light->light_matrix(package.camera, i);
        }

        gpu.context()->PSSetShaderResources(1, DirectionalLight::CASCADE_COUNT, dir_light_views);

        cb.SUN_DIRECTION = directional_light->direction();
        cb.SUN_COLOR = directional_light->color;
        cb.SUN_POINT_SIZE = directional_light->point_size;

        cb.SHADOW_TEXTURE_SIZE = Float2{ (float) directional_light->resolution() };
        cb.SHADOW_TEXTURE_TEXEL_SIZE = Float2{ 1.0f / directional_light->resolution() };
        for (int i = 0; i < DirectionalLight::CASCADE_COUNT; i++) {
            cb.SHADOW_CASCADES[i] = kl::lerp(directional_light->cascade_splits[i + 1], package.camera->near_plane, package.camera->far_plane);
        }
    }
    cb.RECEIVES_SHADOWS = true;
    cb.ELAPSED_TIME = timer.elapsed();
    cb.DELTA_TIME = timer.delta();

    gpu.bind_target_depth_views({
        package.camera->color_texture.target_view.get(),
        package.camera->index_texture.target_view.get(), },
        package.camera->depth_texture.depth_view);
    gpu.bind_sampler_state_for_pixel_shader(render_layer.sampler_states.linear, 0);
    gpu.bind_sampler_state_for_pixel_shader(render_layer.sampler_states.shadow, 1);
    gpu.bind_sampler_state_for_pixel_shader(render_layer.sampler_states.linear, 2);
    if (Texture* skybox = scene.helper_get_texture(package.camera->skybox_texture_name)) {
        gpu.bind_shader_view_for_pixel_shader(skybox->shader_view, 0);
    }
    else {
        gpu.unbind_shader_view_for_pixel_shader(0);
    }

    struct RenderInfo
    {
        int id = 0;
        Entity* entity = nullptr;
        Animation* animation = nullptr;
		Mesh* mesh = nullptr;
		Material* material = nullptr;
        const Texture* color_texture = nullptr;
		const Texture* normal_texture = nullptr;
		const Texture* roughness_texture = nullptr;
        kl::RenderShaders* render_shaders = nullptr;
        float camera_distance = 0.0f;
    };

    Vector<RenderInfo> to_render;
    to_render.reserve(scene.entities().size());

    const auto schedule_entity_helper = [&](const int id, Entity* entity)
    {
        if (!package.camera->can_see(entity->position())) {
            return;
        }

        RenderInfo info{};
        info.id = id;
        info.entity = entity;

        info.animation = scene.helper_get_animation(entity->animation_name);
        if (!info.animation) {
            return;
        }

        info.mesh = info.animation->get_mesh(scene, timer.elapsed());
        info.material = scene.helper_get_material(entity->material_name);
        if (!info.mesh || !info.material) {
            return;
        }

        info.color_texture = scene.helper_get_texture(info.material->color_texture_name);
        info.normal_texture = scene.helper_get_texture(info.material->normal_texture_name);
        info.roughness_texture = scene.helper_get_texture(info.material->roughness_texture_name);

        if (Shader* shader = scene.helper_get_shader(info.material->shader_name)) {
            info.render_shaders = &shader->graphics_buffer;
        }
        else {
            info.render_shaders = &package.shader_state;
        }
        info.camera_distance = (entity->position() - package.camera->position()).length();
        to_render.push_back(info);
    };

    for (int counter = 0; const auto& [_, entity] : scene.entities()) {
        schedule_entity_helper(++counter, &entity);
    }

	std::sort(to_render.begin(), to_render.end(), [](const RenderInfo& a, const RenderInfo& b)
    {
        return a.camera_distance < b.camera_distance;
    });
    
    bool wireframe_bound = package.camera->render_wireframe;
    gpu.bind_raster_state(wireframe_bound ? render_layer.raster_states.wireframe : render_layer.raster_states.solid_cull);

    const auto render_entity_helper = [&](const RenderInfo& info)
    {
        const bool should_wireframe = package.camera->render_wireframe || info.mesh->render_wireframe;
        if (should_wireframe != wireframe_bound) {
            wireframe_bound = should_wireframe;
            gpu.bind_raster_state(wireframe_bound ? render_layer.raster_states.wireframe : render_layer.raster_states.solid_cull);
        }

        if (info.color_texture) {
            gpu.bind_shader_view_for_pixel_shader(info.color_texture->shader_view, 5);
        }
        else {
            gpu.unbind_shader_view_for_pixel_shader(5);
        }
        if (info.normal_texture) {
            gpu.bind_shader_view_for_pixel_shader(info.normal_texture->shader_view, 6);
            cb.HAS_NORMAL_TEXTURE = 1.0f;
        }
        else {
            cb.HAS_NORMAL_TEXTURE = 0.0f;
        }
        if (info.roughness_texture) {
            gpu.bind_shader_view_for_pixel_shader(info.roughness_texture->shader_view, 7);
            cb.HAS_ROUGHNESS_TEXTURE = 1.0f;
        }
        else {
            cb.HAS_ROUGHNESS_TEXTURE = 0.0f;
        }

        cb.W = info.entity->model_matrix();

        cb.OBJECT_INDEX = float(info.id);
        cb.OBJECT_SCALE = info.entity->scale();
        cb.OBJECT_ROTATION = info.entity->rotation();
        cb.OBJECT_POSITION = info.entity->position();

        cb.MATERIAL_COLOR = info.material->color;
        cb.TEXTURE_BLEND = info.material->texture_blend;

        cb.REFLECTIVITY_FACTOR = info.material->reflectivity_factor;
        cb.REFRACTION_INDEX = info.material->refraction_index;

        if (info.animation->animation_type == AnimationType::SKELETAL) {
            info.animation->bind_matrices(0);
            cb.IS_SKELETAL = 1.0f;
        }
        else {
            cb.IS_SKELETAL = 0.0f;
        }

        cb.CUSTOM_DATA = info.material->custom_data;

        if (*info.render_shaders) {
            info.render_shaders->upload(cb);
            gpu.bind_render_shaders(*info.render_shaders);
            gpu.draw(info.mesh->graphics_buffer, info.mesh->topology, sizeof(Vertex));
            bench_add_draw_call();
        }
    };

    for (const auto& info : to_render) {
        if (!info.material->is_transparent()) {
            render_entity_helper(info);
        }
    }

    gpu.bind_depth_state(render_layer.depth_states.only_compare);
    cb.RECEIVES_SHADOWS = false;

    for (int i = (int) to_render.size() - 1; i >= 0; i--) {
        const auto& info = to_render[i];
        if (info.material->is_transparent()) {
            render_entity_helper(info);
        }
    }

    for (int i = 0; i < 8; i++) {
        gpu.unbind_shader_view_for_pixel_shader(i);
    }
}
