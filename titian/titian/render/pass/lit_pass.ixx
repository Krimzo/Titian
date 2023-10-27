export module lit_pass;

export import render_pass;
export import editor_layer;
export import render_layer;
export import render_states;

export import ambient_light;
export import directional_light;

export namespace titian {
    class LitPass : public RenderPass
    {
    public:
        RenderLayer* render_layer = nullptr;
        
        LitPass(GameLayer* game_layer, RenderLayer* render_layer)
            : RenderPass(game_layer)
        {
            this->render_layer = render_layer;
        }

        ~LitPass() override
        {}

        bool is_renderable() const override
        {
            return true;
        }

        StatePackage get_state_package() override
        {
            RenderStates* render_states = &render_layer->states;
            
            StatePackage package = {};
            package.raster_state = render_layer->render_wireframe ? render_states->raster_states->wireframe : render_states->raster_states->solid;
            package.depth_state = render_states->depth_states->enabled;
            package.shader_state = render_states->shader_states->lit_pass;
            return package;
        }

        void render_self(StatePackage& package) override
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

            struct VSData
            {
                kl::Float4x4 w_matrix;
                kl::Float4x4 vp_matrix;
                kl::Float4x4 vp_light_matrices[DirectionalLight::CASCADE_COUNT];
            } vs_data = {};
            vs_data.vp_matrix = camera->camera_matrix();

            struct PSData
            {
                kl::Float4 object_color; // (color.r, color.g, color.b, none)
                kl::Float4 object_index; // (index, index, index, index)
        
                kl::Float4 object_material; // (texture_blend, reflection_factor, refraction_factor, refraction_index)
                kl::Float4 object_texture_info; // (has_normal_map, has_roughness_map, none, none)
        
                kl::Float4 camera_info; // (camera.x, camera.y, camera.z, skybox?)
                kl::Float4 camera_background; // (color.r, color.g, color.b, color.a)
                kl::Float4x4 v_matrix; // View matrix
        
                kl::Float4 ambient_light; // (color.r, color.g, color.b, intensity)
                kl::Float4 directional_light; // (sun.x, sun.y, sun.z, sun_point_size)
        
                kl::Float4 shadow_map_info; // (width, height, texel_width, texel_size)
                kl::Float4 cascade_distances; // (cascade_0_far, cascade_1_far, cascade_2_far, cascade_3_far)
            } ps_data = {};
            ps_data.camera_info = { camera->position(), static_cast<float>(scene->textures.contains(camera->skybox_name)) };
            ps_data.camera_background = camera->background;
            ps_data.v_matrix = camera->view_matrix();

            AmbientLight* ambient_light = scene->get_casted<AmbientLight>(scene->main_ambient_light_name);
            if (ambient_light) {
                ps_data.ambient_light = { ambient_light->color, ambient_light->intensity };
            }
            
            DirectionalLight* directional_light = scene->get_casted<DirectionalLight>(scene->main_directional_light_name);
            if (directional_light) {
                ID3D11ShaderResourceView* dir_light_views[DirectionalLight::CASCADE_COUNT] = {};
                for (int i = 0; i < DirectionalLight::CASCADE_COUNT; i++) {
                    dir_light_views[i] = directional_light->shader_view(i).Get();
                    vs_data.vp_light_matrices[i] = directional_light->light_matrix(camera, i);
                }
            
                gpu->context()->PSSetShaderResources(1, DirectionalLight::CASCADE_COUNT, dir_light_views);
                
                ps_data.directional_light = { directional_light->direction(), directional_light->point_size };  
                ps_data.shadow_map_info = { kl::Float2((float) directional_light->map_resolution()), kl::Float2(1.0f / directional_light->map_resolution()) };
                for (int i = 0; i < DirectionalLight::CASCADE_COUNT; i++) {
                    ps_data.cascade_distances[i] = kl::unwrap(directional_light->CASCADE_SPLITS[i + 1], camera->near_plane, camera->far_plane);
                }
            }
        
            uint32_t id_counter = 0;
            for (const auto& [name, entity] : *scene) {
                id_counter += 1;
                const Mesh* mesh = &scene->get_mesh(entity->mesh_name);
                const Material* material = &scene->get_material(entity->material_name);
                if (!mesh || !material) { continue; }

                const Texture* color_map = &scene->get_texture(material->color_map_name);
                if (color_map) {
                    gpu->bind_shader_view_for_pixel_shader(color_map->shader_view, 5);
                }
        
                const Texture* normal_map = &scene->get_texture(material->normal_map_name);
                if (normal_map) {
                    gpu->bind_shader_view_for_pixel_shader(normal_map->shader_view, 6);
                    ps_data.object_texture_info.x = 1.0f;
                }
                else {
                    ps_data.object_texture_info.x = 0.0f;
                }
        
                const Texture* roughness_map = &scene->get_texture(material->roughness_map_name);
                if (roughness_map) {
                    gpu->bind_shader_view_for_pixel_shader(roughness_map->shader_view, 7);
                    ps_data.object_texture_info.y = 1.0f;
                }
                else {
                    ps_data.object_texture_info.y = 0.0f;
                }
        
                vs_data.w_matrix = entity->model_matrix();
        
                ps_data.object_color = material->color;
                ps_data.object_index = kl::Float4 { static_cast<float>(id_counter) };
                ps_data.object_material = {
                    material->texture_blend,
                    material->reflection_factor,
                    material->refraction_factor,
                    material->refraction_index,
                };

                // Draw
                package.shader_state.vertex_shader.update_cbuffer(vs_data);
                package.shader_state.pixel_shader.update_cbuffer(ps_data);
                gpu->draw(mesh->graphics_buffer);
            }

            // Unbind shadow maps
            {
                ID3D11ShaderResourceView* dir_light_views[DirectionalLight::CASCADE_COUNT] = {};
                gpu->context()->PSSetShaderResources(1, DirectionalLight::CASCADE_COUNT, dir_light_views);
            }
        }
    };
}
