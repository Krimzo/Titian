export module shadow_pass;

export import render_pass;
export import game_layer;
export import render_layer;

export import directional_light;

export namespace titian {
    class ShadowPass : public RenderPass
    {
    public:
        kl::Object<RenderLayer> render_layer = nullptr;

        ShadowPass(kl::Object<GameLayer>& game_layer, kl::Object<RenderLayer>& render_layer)
            : RenderPass(game_layer)
        {
            this->render_layer = render_layer;
        }

        ~ShadowPass() override
        {}

        bool is_renderable() const override
        {
            const Scene* scene = &game_layer->scene;
            const DirectionalLight* dir_light = scene->get_dynamic<DirectionalLight>(scene->directional_light);
            return static_cast<bool>(dir_light);
        }

        StatePackage get_state_package() override
        {
            RenderStates* render_states = &render_layer->states;

            StatePackage package = {};
            package.raster_state = render_states->raster_states->shadow;
            package.depth_state = render_states->depth_states->enabled;
            package.shader_state = render_states->shader_states->shadow_pass;
            return package;
        }

        void render_self(StatePackage& package) override
        {
            kl::GPU* gpu = &game_layer->app_layer->gpu;
            Scene* scene = &game_layer->scene;
            
            Camera* camera = scene->get_dynamic<Camera>(scene->camera);
            if (!camera) { return; }
            
            DirectionalLight* dir_light = scene->get_dynamic<DirectionalLight>(scene->directional_light);
            if (!dir_light) { return; }
            
            gpu->set_viewport_size(kl::Int2{ (int) dir_light->map_resolution });
            for (int i = 0; i < kl::DirectionalLight::CASCADE_COUNT; i++) {
                const kl::Float4x4 VP = dir_light->light_matrix(camera, i);
                const kl::dx::DepthView shadow_map = dir_light->depth_view(i);
            
                gpu->bind_target_depth_views({}, shadow_map);
                gpu->clear_depth_view(shadow_map, 1.0f, 0xFF);
                
                for (auto& [_, entity] : *scene) {
                    Mesh* mesh = &scene->get_mesh(entity->mesh);
                    if (!mesh) { continue; }
                
                    struct VSData
                    {
                        kl::Float4x4 WVP;
                    } vs_data = {};
                    vs_data.WVP = VP * entity->model_matrix();
                
                    // Draw
                    package.shader_state.vertex_shader.update_cbuffer(vs_data);
                    gpu->draw(mesh->graphics_buffer);
                }
            }
        }
    };
}
