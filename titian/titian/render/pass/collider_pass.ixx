export module collider_pass;

export import render_pass;
export import editor_layer;
export import gui_layer;

export namespace titian {
    class ColliderPass : public RenderPass
    {
    public:
        kl::Object<EditorLayer> editor_layer = nullptr;
        kl::Object<GUILayer> gui_layer = nullptr;

        ColliderPass(kl::Object<GameLayer>& game_layer, kl::Object<EditorLayer>& editor_layer, kl::Object<GUILayer>& gui_layer)
            : RenderPass(game_layer)
        {
            this->editor_layer = editor_layer;
            this->gui_layer = gui_layer;
        }

        ~ColliderPass() override
        {}

        bool is_renderable() const override
        {
            const Scene* scene = &game_layer->scene;
            const Entity* entity = &scene->get_entity(editor_layer->selected_entity);
            return entity && entity->collider();
        }

        StatePackage get_state_package() override
        {
            RenderStates* render_states = &gui_layer->render_layer->states;

            StatePackage package = {};
            package.raster_state = render_states->raster_states->wireframe;
            package.depth_state = render_states->depth_states->enabled;
            package.shader_state = render_states->shader_states->unlit_pass;
            return package;
        }

        void render_self(StatePackage& package) override
        {
            kl::GPU* gpu = &game_layer->app_layer->gpu;
            Scene* scene = &game_layer->scene;

            Camera* camera = scene->get_casted<Camera>(scene->main_camera_name);
            if (!camera) { return; }

            Entity* entity = scene->get_casted<Entity>(editor_layer->selected_entity);
            if (!entity) { return; }

            struct VSData
            {
                kl::Float4x4 wvp_matrix;
            } vs_data = {};
            vs_data.wvp_matrix = camera->camera_matrix() * entity->collider_matrix();

            struct PSData
            {
                kl::Float4 object_color; // (color.r, color.g, color.b, none)
            } ps_data = {};
            ps_data.object_color = gui_layer->alternate_color;

            package.shader_state.vertex_shader.update_cbuffer(vs_data);
            package.shader_state.pixel_shader.update_cbuffer(ps_data);

            switch (entity->collider()->type()) {
            case physx::PxGeometryType::Enum::eBOX:
                gpu->draw(game_layer->scene->default_meshes->cube->graphics_buffer);
                break;

            case physx::PxGeometryType::Enum::eSPHERE:
                gpu->draw(game_layer->scene->default_meshes->sphere->graphics_buffer);
                break;

            case physx::PxGeometryType::Enum::eCAPSULE:
                gpu->draw(game_layer->scene->default_meshes->capsule->graphics_buffer);
                break;

            case physx::PxGeometryType::Enum::eTRIANGLEMESH:
                Mesh* mesh = &scene->get_mesh(entity->mesh_name);
                if (mesh) {
                    gpu->draw(mesh->graphics_buffer);
                }
                break;
            }
        }
    };
}
