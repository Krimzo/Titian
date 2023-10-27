export module display_pass;

export import render_pass;
export import render_layer;

export namespace titian {
    class DisplayPass : public RenderPass
    {
    public:
        RenderLayer* render_layer = nullptr;

        DisplayPass(GameLayer* game_layer, RenderLayer* render_layer)
            : RenderPass(game_layer)
        {
            this->render_layer = render_layer;
        }

        ~DisplayPass() override
        {}

        bool is_renderable() const override
        {
            return true;
        }

        StatePackage get_state_package() override
        {
            RenderStates* render_states = &render_layer->states;

            StatePackage package = {};
            package.raster_state = render_states->raster_states->solid;
            package.depth_state = render_states->depth_states->disabled;
            package.shader_state = render_states->shader_states->display_pass;
            return package;
        }

        void render_self(StatePackage& package) override
        {
            kl::Window* window = &game_layer->app_layer->window;
            kl::GPU* gpu = &game_layer->app_layer->gpu;

            const kl::Int2 window_size = window->size();
            render_layer->resize(window_size);

            gpu->bind_internal_views();
            gpu->set_viewport_size(window_size);

            gpu->bind_shader_view_for_pixel_shader(render_layer->render_texture->shader_view, 0);
            gpu->draw(render_layer->screen_mesh);
        }
    };
}
