export module titian_editor;

export import packager;
export import layer_stack;

export import app_layer;
export import game_layer;
export import editor_layer;
export import render_layer;
export import gui_layer;

export import shadow_pass;
export import skybox_pass;
export import lit_pass;
export import collider_pass;
export import postprocess_pass;

export import section_main_menu;
export import section_scene_entities;
export import section_scene_info;
export import section_mesh_editor;
export import section_material_editor;
export import section_script_editor;
export import section_viewport;
export import section_control_menu;
export import section_log_view;
export import section_explorer;
export import section_entity_properties;
export import section_render_info;

export namespace titian {
    class TitianEditor : public LayerStack
    {
    public:
        kl::Object<AppLayer> app_layer = nullptr;
        kl::Object<GameLayer> game_layer = nullptr;
        kl::Object<EditorLayer> editor_layer = nullptr;
        kl::Object<RenderLayer> render_layer = nullptr;
        kl::Object<GUILayer> gui_layer = nullptr;

        TitianEditor()
        {
            // Unpackage
            open_package(PACKAGED_BUILTIN_FILE);

            // Init layers
            app_layer = new AppLayer("Titian Editor");
            game_layer = new GameLayer(&app_layer);
            editor_layer = new EditorLayer(&game_layer);
            render_layer = new RenderLayer(&game_layer);
            gui_layer = new GUILayer(&render_layer);

            // Bind
            game_layer->bind_self();

            // Init render passes
            render_layer->passes.emplace_back(new ShadowPass(&game_layer, &render_layer));
            render_layer->passes.emplace_back(new SkyboxPass(&game_layer, &render_layer));
            render_layer->passes.emplace_back(new LitPass(&game_layer, &render_layer));
            render_layer->passes.emplace_back(new ColliderPass(&game_layer, &editor_layer, &gui_layer));
            render_layer->passes.emplace_back(new PostprocessPass(&game_layer, &editor_layer, &gui_layer));

            // Init editor sections
            gui_layer->sections.emplace_back(new GUISectionMainMenu(&render_layer));
            gui_layer->sections.emplace_back(new GUISectionSceneEntities(&editor_layer));
            gui_layer->sections.emplace_back(new GUISectionSceneInfo(&editor_layer));
            gui_layer->sections.emplace_back(new GUISectionMeshEditor(&editor_layer, &gui_layer));
            gui_layer->sections.emplace_back(new GUISectionMaterialEditor(&editor_layer, &gui_layer));
            gui_layer->sections.emplace_back(new GUISectionScriptEditor(&editor_layer, &gui_layer));
            gui_layer->sections.emplace_back(new GUISectionViewport(&editor_layer, &render_layer));
            gui_layer->sections.emplace_back(new GUISectionControlMenu(&editor_layer));
            gui_layer->sections.emplace_back(new GUISectionLogView(&gui_layer));
            gui_layer->sections.emplace_back(new GUISectionExplorer(&app_layer));
            gui_layer->sections.emplace_back(new GUISectionEntityProperties(&editor_layer, &gui_layer));
            gui_layer->sections.emplace_back(new GUISectionRenderInfo(&game_layer));

            // Push layers
            push_layer(app_layer);
            push_layer(game_layer);
            push_layer(editor_layer);
            push_layer(render_layer);
            push_layer(gui_layer);
        }

        ~TitianEditor() override
        {
            gui_layer->sections.clear();
            render_layer->passes.clear();
        }
    };
}
