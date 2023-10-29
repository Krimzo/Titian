export module editor_pp_pass;

export import render_pass;
export import editor_layer;
export import gui_layer;

export namespace titian {
    class EditorPPPass : public RenderPass
    {
    public:
        EditorLayer* editor_layer = nullptr;
        GUILayer* gui_layer = nullptr;

        EditorPPPass(GameLayer* game_layer, EditorLayer* editor_layer, GUILayer* gui_layer)
            : RenderPass(game_layer)
        {
            this->editor_layer = editor_layer;
            this->gui_layer = gui_layer;
        }

        ~EditorPPPass() override
        {}

        bool is_renderable() const override
        {
            const Scene* scene = &game_layer->scene;
            return scene->get_entity(editor_layer->selected_entity);
        }

        StatePackage get_state_package() override
        {
            RenderStates* render_states = &gui_layer->render_layer->states;

            StatePackage package = {};
            package.raster_state = render_states->raster_states->solid;
            package.depth_state = render_states->depth_states->disabled;
            package.shader_state = render_states->shader_states->pp_pass;
            return package;
        }

        void render_self(StatePackage& package) override
        {
            RenderLayer* render_layer = gui_layer->render_layer;
            kl::GPU* gpu = &game_layer->app_layer->gpu;
            Scene* scene = &game_layer->scene;

            Entity* entity = &scene->get_entity(editor_layer->selected_entity);
            if (!entity) { return; }

            uint32_t counter_id = 0;
            for (auto& [_, ent] : *editor_layer->game_layer->scene) {
                counter_id += 1;
                if (&ent == entity) {
                    break;
                }
            }

            gpu->bind_target_depth_views({ render_layer->render_texture->target_view }, render_layer->depth_texture->depth_view);
            gpu->bind_shader_view_for_pixel_shader(render_layer->picking_texture->shader_view, 0);

            struct PSData
            {
                kl::Float4 selected_index;
                kl::Float4 outline_color;
            } ps_data = {};
            ps_data.selected_index = kl::Float4{ static_cast<float>(counter_id) };
            ps_data.outline_color = gui_layer->special_color;

            package.shader_state.pixel_shader.update_cbuffer(ps_data);
            gpu->draw(render_layer->screen_mesh);
        }
    };
}
