export module skybox_pass;

export import render_pass;
export import render_states;

export namespace titian {
    class SkyboxPass : public RenderPass
    {
    public:
        class VSData
        {
        public:
            kl::Float4x4 vp_matrix = {};
        };

        kl::Object<RenderStates> render_states = nullptr;

        SkyboxPass(kl::Object<GameLayer>& game_layer, kl::Object<RenderStates>& render_states)
            : RenderPass(game_layer)
        {
            this->render_states = render_states;
        }

        ~SkyboxPass() override
        {}

        bool is_renderable() const override
        {
            const Scene* scene = &game_layer->scene;
            return scene->textures.contains(scene->camera->skybox);
        }

        StatePackage get_state_package() override
        {
            StatePackage package = {};
            package.raster_state = render_states->raster_states->solid;
            package.depth_state = render_states->depth_states->disabled;
            package.shader_state = render_states->shader_states->skybox_pass;
            return package;
        }

        void render_self(StatePackage& package) override
        {
            // Helper
            kl::GPU* gpu = &game_layer->app_layer->gpu;
            Scene* scene = &game_layer->scene;

            // Set cb data
            VSData vs_data = {};
            vs_data.vp_matrix = scene->camera->matrix();
            package.shader_state.vertex_shader.update_cbuffer(vs_data);

            // Bind other states
            kl::Object<Texture> skybox = scene->textures.at(scene->camera->skybox);
            gpu->bind_sampler_state_for_pixel_shader(render_states->sampler_states->linear, 0);
            gpu->bind_shader_view_for_pixel_shader(skybox->shader_view, 0);

            // Draw call
            gpu->draw(scene->default_meshes->cube->graphics_buffer);
        }
    };
}
