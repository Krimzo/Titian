#include "main.h"


titian::ScenePass::ScenePass(GameLayer* game_layer, RenderLayer* render_layer)
    : RenderPass(game_layer)
{
    this->render_layer = render_layer;
}

bool titian::ScenePass::is_renderable() const
{
    return true;
}

titian::StatePackage titian::ScenePass::get_state_package()
{
    RenderStates* render_states = &render_layer->states;

    StatePackage package = {};
    package.raster_state = render_layer->render_wireframe ? render_states->raster_states->wireframe : nullptr;
    package.depth_state = render_states->depth_states->enabled;
    package.shader_state = render_states->shader_states->scene_pass;
    return package;
}

void titian::ScenePass::render_self(StatePackage& package)
{
    RenderStates* render_states = &render_layer->states;
    kl::GPU* gpu = &game_layer->app_layer->gpu;
    Scene* scene = &game_layer->scene;

    Camera* camera = scene->get_casted<Camera>(scene->main_camera_name);
    if (!camera) { return; }

    gpu->bind_sampler_state_for_pixel_shader(render_states->sampler_states->linear, 0);
    gpu->bind_sampler_state_for_pixel_shader(render_states->sampler_states->shadow, 1);
    gpu->bind_sampler_state_for_pixel_shader(render_states->sampler_states->linear, 2);

    if (Texture* skybox = &scene->get_texture(camera->skybox_name)) {
        gpu->bind_shader_view_for_pixel_shader(skybox->shader_view, 0);
    }
    else {
        gpu->unbind_shader_view_for_pixel_shader(0);
    }

    struct VS_CB
    {
        kl::Float4x4 W;
        kl::Float4x4 VP;
        kl::Float4x4 LIGHT_VPs[DirectionalLight::CASCADE_COUNT];
    };

    VS_CB vs_cb{};
    vs_cb.VP = camera->camera_matrix();

    struct PS_CB
    {
        kl::Float4 OBJECT_COLOR; // (color.r, color.g, color.b, none)
        kl::Float4 OBJECT_INDEX; // (index, index, index, index)

        kl::Float4 OBJECT_MATERIAL; // (texture_blend, reflection_factor, refraction_factor, refraction_index)
        kl::Float4 OBJECT_TEXTURE_INFO; // (has_normal_map, has_roughness_map, none, none)

        kl::Float4 CAMERA_INFO; // (camera.x, camera.y, camera.z, skybox?)
        kl::Float4 CAMERA_BACKGROUND; // (color.r, color.g, color.b, color.a)
        kl::Float4x4 V; // View matrix

        kl::Float4 AMBIENT_LIGHT; // (color.r, color.g, color.b, intensity)
        kl::Float4 DIRECTIONAL_LIGHT; // (dir.x, dir.y, dir.z, sun_point_size)
        kl::Float4 DIRECTIONAL_LIGHT_COLOR; // (color.r, color.g, color.b, none)

        kl::Float4 SHADOW_MAP_INFO; // (width, height, texel_width, texel_size)
        kl::Float4 CASCADE_DISTANCES; // (cascade_0_far, cascade_1_far, cascade_2_far, cascade_3_far)
    };

    PS_CB ps_cb{};
    ps_cb.CAMERA_INFO = { camera->position(), static_cast<float>(scene->textures.contains(camera->skybox_name)) };
    ps_cb.CAMERA_BACKGROUND = camera->background;
    ps_cb.V = camera->view_matrix();

    AmbientLight* ambient_light = scene->get_casted<AmbientLight>(scene->main_ambient_light_name);
    if (ambient_light) {
        ps_cb.AMBIENT_LIGHT = { ambient_light->color, ambient_light->intensity };
    }

    DirectionalLight* directional_light = scene->get_casted<DirectionalLight>(scene->main_directional_light_name);
    if (directional_light) {
        ID3D11ShaderResourceView* dir_light_views[DirectionalLight::CASCADE_COUNT] = {};
        for (int i = 0; i < DirectionalLight::CASCADE_COUNT; i++) {
            dir_light_views[i] = directional_light->shader_view(i).Get();
            vs_cb.LIGHT_VPs[i] = directional_light->light_matrix(camera, i);
        }

        gpu->context()->PSSetShaderResources(1, DirectionalLight::CASCADE_COUNT, dir_light_views);

        ps_cb.DIRECTIONAL_LIGHT = { directional_light->direction(), directional_light->point_size };
        ps_cb.DIRECTIONAL_LIGHT_COLOR = { directional_light->color, {} };

        ps_cb.SHADOW_MAP_INFO = { kl::Float2((float)directional_light->map_resolution()), kl::Float2(1.0f / directional_light->map_resolution()) };
        for (int i = 0; i < DirectionalLight::CASCADE_COUNT; i++) {
            ps_cb.CASCADE_DISTANCES[i] = kl::unwrap(directional_light->CASCADE_SPLITS[i + 1], camera->near_plane, camera->far_plane);
        }
    }

    uint32_t id_counter = 0;
    for (const auto& [name, entity] : *scene) {
        id_counter += 1;

        const Mesh* mesh = &scene->get_mesh(entity->mesh_name);
        const Material* material = &scene->get_material(entity->material_name);
        if (!mesh || !material) { continue; }

        if (!render_layer->render_wireframe) {
            gpu->bind_raster_state(mesh->render_wireframe ? render_states->raster_states->wireframe : render_states->raster_states->solid);
        }

        const Texture* color_map = &scene->get_texture(material->color_map_name);
        if (color_map) {
            gpu->bind_shader_view_for_pixel_shader(color_map->shader_view, 5);
        }

        const Texture* normal_map = &scene->get_texture(material->normal_map_name);
        if (normal_map) {
            gpu->bind_shader_view_for_pixel_shader(normal_map->shader_view, 6);
            ps_cb.OBJECT_TEXTURE_INFO.x = 1.0f;
        }
        else {
            ps_cb.OBJECT_TEXTURE_INFO.x = 0.0f;
        }

        const Texture* roughness_map = &scene->get_texture(material->roughness_map_name);
        if (roughness_map) {
            gpu->bind_shader_view_for_pixel_shader(roughness_map->shader_view, 7);
            ps_cb.OBJECT_TEXTURE_INFO.y = 1.0f;
        }
        else {
            ps_cb.OBJECT_TEXTURE_INFO.y = 0.0f;
        }

        vs_cb.W = entity->model_matrix();

        ps_cb.OBJECT_COLOR = material->color;
        ps_cb.OBJECT_INDEX = kl::Float4{ static_cast<float>(id_counter) };
        ps_cb.OBJECT_MATERIAL = {
            material->texture_blend,
            material->reflection_factor,
            material->refraction_factor,
            material->refraction_index,
        };

        // Draw
        package.shader_state.vertex_shader.update_cbuffer(vs_cb);
        package.shader_state.pixel_shader.update_cbuffer(ps_cb);
        gpu->draw(mesh->graphics_buffer, mesh->casted_topology());
    }

    // Unbind shadow maps
    {
        ID3D11ShaderResourceView* dir_light_views[DirectionalLight::CASCADE_COUNT] = {};
        gpu->context()->PSSetShaderResources(1, DirectionalLight::CASCADE_COUNT, dir_light_views);
    }
}
