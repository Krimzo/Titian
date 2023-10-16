export module skybox_pass;

export import render_pass;
export import render_layer;

export namespace titian {
    class SkyboxPass : public RenderPass
    {
    public:
        kl::Object<RenderLayer> render_layer = nullptr;

        SkyboxPass(kl::Object<GameLayer>& game_layer, kl::Object<RenderLayer>& render_layer)
            : RenderPass(game_layer)
        {
            this->render_layer = render_layer;
        }

        ~SkyboxPass() override
        {}

        bool is_renderable() const override
        {
            const Scene* scene = &game_layer->scene;
            const Camera* camera = scene->get_dynamic<Camera>(scene->main_camera_name);
            return static_cast<bool>(camera);
        }

        StatePackage get_state_package() override
        {
            RenderStates* render_states = &render_layer->states;
            
            StatePackage package = {};
            package.raster_state = render_states->raster_states->solid;
            package.depth_state = render_states->depth_states->disabled;
            package.shader_state = render_states->shader_states->skybox_pass;
            return package;
        }

        void render_self(StatePackage& package) override
        {
            // Helper
            RenderStates* render_states = &render_layer->states;
            kl::GPU* gpu = &game_layer->app_layer->gpu;
            Scene* scene = &game_layer->scene;

            Camera* camera = scene->get_dynamic<Camera>(scene->main_camera_name);
            if (!camera) { return; }

            Texture* skybox = &scene->get_texture(camera->skybox_name);
            if (!skybox) { return; }

            // Set cb data
            struct VSData
            {
                kl::Float4x4 vp_matrix = {};
            } vs_data = {};
            vs_data.vp_matrix = camera->camera_matrix();
            package.shader_state.vertex_shader.update_cbuffer(vs_data);

            // Draw
            gpu->bind_sampler_state_for_pixel_shader(render_states->sampler_states->linear, 0);
            gpu->bind_shader_view_for_pixel_shader(skybox->shader_view, 0);
            gpu->draw(scene->default_meshes->cube->graphics_buffer);
        }
    };
}
