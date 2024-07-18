#include "main.h"


titian::ScenePass::ScenePass(const LayerPackage& package)
    : RenderPass("ScenePass", package)
{}

bool titian::ScenePass::is_renderable() const
{
    return true;
}

titian::StatePackage titian::ScenePass::get_state_package()
{
    RenderStates* render_states = &render_layer->states;

    StatePackage package{};
    package.raster_state = render_layer->render_wireframe ? render_states->raster_states->wireframe : nullptr;
    package.depth_state = render_states->depth_states->enabled;
    package.shader_state = render_states->shader_states->scene_pass;
    package.blend_state = render_states->blend_states->enabled;
    return package;
}

void titian::ScenePass::render_self(StatePackage& package)
{
    // Helper
    RenderStates* render_states = &render_layer->states;
    kl::GPU* gpu = &app_layer->gpu;
    Scene* scene = &game_layer->scene;

    // Skip if no camera
    Camera* camera = scene->get_casted<Camera>(scene->main_camera_name);
    if (!camera)
        return;

    // Target
    gpu->bind_target_depth_views({ render_layer->game_color_texture->target_view.Get(), render_layer->editor_picking_texture->target_view.Get() }, render_layer->game_depth_texture->depth_view);

    // Bind shader views
    gpu->bind_sampler_state_for_pixel_shader(render_states->sampler_states->linear, 0);
    gpu->bind_sampler_state_for_pixel_shader(render_states->sampler_states->shadow, 1);
    gpu->bind_sampler_state_for_pixel_shader(render_states->sampler_states->linear, 2);

    // Bind skybox
    if (Texture* skybox = &scene->get_texture(camera->skybox_name)) {
        gpu->bind_shader_view_for_pixel_shader(skybox->shader_view, 0);
    }
    else {
        gpu->unbind_shader_view_for_pixel_shader(0);
    }

    // Set cb data
    struct GLOBAL_CB
    {
        float ELAPSED_TIME{};
        float DELTA_TIME{};

        alignas(16) kl::Float3 CAMERA_POSITION;
        float CAMERA_HAS_SKYBOX{};
        kl::Float4 CAMERA_BACKGROUND;

        kl::Float3 AMBIENT_COLOR;
        float AMBIENT_INTENSITY{};

        kl::Float3 SUN_DIRECTION;
        float SUN_POINT_SIZE{};
        kl::Float3 SUN_COLOR;

        float OBJECT_INDEX{};
        kl::Float3 OBJECT_SCALE;
        alignas(16) kl::Float3 OBJECT_ROTATION;
        alignas(16) kl::Float3 OBJECT_POSITION;

        alignas(16) kl::Float4 OBJECT_COLOR;
        float TEXTURE_BLEND{};

        float REFLECTION_FACTOR{};
        float REFRACTION_FACTOR{};
        float REFRACTION_INDEX{};

        float HAS_NORMAL_MAP{};
        float HAS_ROUGHNESS_MAP{};

        alignas(16) kl::Float4x4 W;
        kl::Float4x4 V;
        kl::Float4x4 VP;

        float RECEIVES_SHADOWS{};
        kl::Float2 SHADOW_MAP_SIZE;
        alignas(16) kl::Float2 SHADOW_MAP_TEXEL_SIZE;
        alignas(16) kl::Float4 SHADOW_CASCADES;
        kl::Float4x4 LIGHT_VPs[DirectionalLight::CASCADE_COUNT];

        kl::Float4x4 BONE_MATRICES[MAX_BONE_COUNT];
        float IS_SKELETAL{};

        alignas(16) kl::Float4x4 CUSTOM_DATA;
    };
    GLOBAL_CB global_cb{};

    global_cb.V = camera->view_matrix();
    global_cb.VP = camera->camera_matrix();
    global_cb.CAMERA_POSITION = camera->position();
    global_cb.CAMERA_HAS_SKYBOX = static_cast<float>(scene->textures.contains(camera->skybox_name));
    global_cb.CAMERA_BACKGROUND = camera->background;

    // Ambient light
    AmbientLight* ambient_light = scene->get_casted<AmbientLight>(scene->main_ambient_light_name);
    if (ambient_light) {
        global_cb.AMBIENT_COLOR = ambient_light->color;
        global_cb.AMBIENT_INTENSITY = ambient_light->intensity;
    }

    // Directional light
    DirectionalLight* directional_light = scene->get_casted<DirectionalLight>(scene->main_directional_light_name);
    if (directional_light) {
        ID3D11ShaderResourceView* dir_light_views[DirectionalLight::CASCADE_COUNT] = {};
        for (int i = 0; i < DirectionalLight::CASCADE_COUNT; i++) {
            dir_light_views[i] = directional_light->shader_view(i).Get();
            global_cb.LIGHT_VPs[i] = directional_light->light_matrix(camera, i);
        }

        gpu->context()->PSSetShaderResources(1, DirectionalLight::CASCADE_COUNT, dir_light_views);

        global_cb.SUN_DIRECTION = directional_light->direction();
        global_cb.SUN_COLOR = directional_light->color;
        global_cb.SUN_POINT_SIZE = directional_light->point_size;

        global_cb.SHADOW_MAP_SIZE = kl::Float2((float) directional_light->map_resolution());
        global_cb.SHADOW_MAP_TEXEL_SIZE = kl::Float2(1.0f / directional_light->map_resolution());
        for (int i = 0; i < DirectionalLight::CASCADE_COUNT; i++) {
            global_cb.SHADOW_CASCADES[i] = kl::unwrap(directional_light->CASCADE_SPLITS[i + 1], camera->near_plane, camera->far_plane);
        }
    }
    global_cb.RECEIVES_SHADOWS = true;

    // Set cb time data
    const kl::Timer* timer = &app_layer->timer;
    global_cb.ELAPSED_TIME = timer->elapsed();
    global_cb.DELTA_TIME = timer->delta();

    // Preapare opaque objects
    std::vector<std::tuple<uint32_t, Entity*, Animation*, Material*>> transparent_objects;
    uint32_t id_counter = 0;

    // Render opaque objects
    for (auto& [_, entity] : *scene) {
        id_counter += 1;

        // Skip
        Animation* animation = &scene->get_animation(entity->animation_name);
		if (!animation) {
			continue;
		}

        Mesh* mesh = animation->get_mesh(timer->elapsed());
        Material* material = &scene->get_material(entity->material_name);
        if (!mesh || !material) {
            continue;
        }

        // Cache transparent objects
        if (material->is_transparent()) {
            transparent_objects.emplace_back(id_counter, &entity, animation, material);
            continue;
        }

        // Bind raster state
        if (!render_layer->render_wireframe) {
            gpu->bind_raster_state(mesh->render_wireframe ? render_states->raster_states->wireframe : render_states->raster_states->solid);
        }

        // Bind textures
        const Texture* color_map = &scene->get_texture(material->color_map_name);
        if (color_map) {
            gpu->bind_shader_view_for_pixel_shader(color_map->shader_view, 5);
        }
        else {
            gpu->unbind_shader_view_for_pixel_shader(5);
        }

        const Texture* normal_map = &scene->get_texture(material->normal_map_name);
        if (normal_map) {
            gpu->bind_shader_view_for_pixel_shader(normal_map->shader_view, 6);
            global_cb.HAS_NORMAL_MAP = 1.0f;
        }
        else {
            global_cb.HAS_NORMAL_MAP = 0.0f;
        }

        const Texture* roughness_map = &scene->get_texture(material->roughness_map_name);
        if (roughness_map) {
            gpu->bind_shader_view_for_pixel_shader(roughness_map->shader_view, 7);
            global_cb.HAS_ROUGHNESS_MAP = 1.0f;
        }
        else {
            global_cb.HAS_ROUGHNESS_MAP = 0.0f;
        }

        // Bind cbuffer
        global_cb.W = entity->model_matrix();

        global_cb.OBJECT_INDEX = static_cast<float>(id_counter);
        global_cb.OBJECT_SCALE = entity->scale;
        global_cb.OBJECT_ROTATION = entity->rotation();
        global_cb.OBJECT_POSITION = entity->position();

        global_cb.OBJECT_COLOR = material->color;
        global_cb.TEXTURE_BLEND = material->texture_blend;

        global_cb.REFLECTION_FACTOR = material->reflection_factor;
        global_cb.REFRACTION_FACTOR = material->refraction_factor;
        global_cb.REFRACTION_INDEX = material->refraction_index;

        if (animation->type == AnimationType::SKELETAL) {
            animation->load_matrices(global_cb.BONE_MATRICES);
            global_cb.IS_SKELETAL = 1.0f;
        }
        else {
            global_cb.IS_SKELETAL = 0.0f;
        }

        global_cb.CUSTOM_DATA = material->custom_data;

        kl::RenderShaders* render_shaders = &package.shader_state;
        if (Shader* shader = &scene->get_shader(material->shader_name)) {
            render_shaders = &shader->graphics_buffer;
        }
        if (render_shaders && *render_shaders) {
            render_shaders->vertex_shader.update_cbuffer(global_cb);
            render_shaders->pixel_shader.update_cbuffer(global_cb);
            gpu->bind_render_shaders(*render_shaders);

            // Draw
            gpu->draw(mesh->graphics_buffer, mesh->casted_topology(), sizeof(Vertex));
        }
    }

    // Prepare transparent objects
    std::sort(transparent_objects.begin(), transparent_objects.end(), [&](const auto& first, const auto& second)
    {
        const float first_distance = (std::get<1>(first)->position() - camera->position()).length();
        const float second_distance = (std::get<1>(second)->position() - camera->position()).length();
        return first_distance > second_distance;
    });
    gpu->bind_depth_state(render_states->depth_states->only_compare);
    global_cb.RECEIVES_SHADOWS = false;

    // Render transparent objects
    for (const auto& [object_id, entity, animation, material] : transparent_objects) {
        Mesh* mesh = animation->get_mesh(timer->elapsed());
		if (!mesh) {
			continue;
		}

        // Bind raster state
        if (!render_layer->render_wireframe) {
            gpu->bind_raster_state(mesh->render_wireframe ? render_states->raster_states->wireframe : render_states->raster_states->solid);
        }

        // Bind textures
        const Texture* color_map = &scene->get_texture(material->color_map_name);
        if (color_map) {
            gpu->bind_shader_view_for_pixel_shader(color_map->shader_view, 5);
        }

        const Texture* normal_map = &scene->get_texture(material->normal_map_name);
        if (normal_map) {
            gpu->bind_shader_view_for_pixel_shader(normal_map->shader_view, 6);
            global_cb.HAS_NORMAL_MAP = 1.0f;
        }
        else {
            global_cb.HAS_NORMAL_MAP = 0.0f;
        }

        const Texture* roughness_map = &scene->get_texture(material->roughness_map_name);
        if (roughness_map) {
            gpu->bind_shader_view_for_pixel_shader(roughness_map->shader_view, 7);
            global_cb.HAS_ROUGHNESS_MAP = 1.0f;
        }
        else {
            global_cb.HAS_ROUGHNESS_MAP = 0.0f;
        }

        // Bind cbuffer
        global_cb.W = entity->model_matrix();

        global_cb.OBJECT_INDEX = static_cast<float>(object_id);
        global_cb.OBJECT_SCALE = entity->scale;
        global_cb.OBJECT_ROTATION = entity->rotation();
        global_cb.OBJECT_POSITION = entity->position();

        global_cb.OBJECT_COLOR = material->color;
        global_cb.TEXTURE_BLEND = material->texture_blend;

        global_cb.REFLECTION_FACTOR = material->reflection_factor;
        global_cb.REFRACTION_FACTOR = material->refraction_factor;
        global_cb.REFRACTION_INDEX = material->refraction_index;

        if (animation->type == AnimationType::SKELETAL) {
            animation->load_matrices(global_cb.BONE_MATRICES);
            global_cb.IS_SKELETAL = 1.0f;
        }
        else {
            global_cb.IS_SKELETAL = 0.0f;
        }

        kl::RenderShaders* render_shaders = &package.shader_state;
        if (Shader* shader = &scene->get_shader(material->shader_name)) {
            render_shaders = &shader->graphics_buffer;
        }
        render_shaders->vertex_shader.update_cbuffer(global_cb);
        render_shaders->pixel_shader.update_cbuffer(global_cb);
        gpu->bind_render_shaders(*render_shaders);

        // Draw
        gpu->draw(mesh->graphics_buffer, mesh->casted_topology(), sizeof(Vertex));
    }

    // Unbind shadow maps
    {
        ID3D11ShaderResourceView* dir_light_views[DirectionalLight::CASCADE_COUNT] = {};
        gpu->context()->PSSetShaderResources(1, DirectionalLight::CASCADE_COUNT, dir_light_views);
    }
}
