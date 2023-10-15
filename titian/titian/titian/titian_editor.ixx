export module titian_editor;

export import layer_stack;
export import application_layer;
export import game_layer;
export import editor_layer;
export import scripting_layer;
export import render_layer;
export import gui_layer;

export import section_main_menu;
export import section_log_view;
export import section_scene_items;
export import section_scene_info;
export import section_viewport;
export import section_explorer;
export import section_control_menu;

export import shadow_pass;
export import skybox_pass;
export import lit_pass;
export import collider_pass;
export import postprocess_pass;

export namespace titian {
    class TitianEditor : public LayerStack
    {
    public:
        kl::Object<ApplicationLayer> app_layer = new ApplicationLayer("Titian");
        kl::Object<GameLayer> game_layer = new GameLayer(app_layer);
        kl::Object<EditorLayer> editor_layer = new EditorLayer(game_layer);
        kl::Object<ScriptingLayer> scripting_layer = new ScriptingLayer(game_layer);
        kl::Object<RenderLayer> render_layer = new RenderLayer(game_layer);
        kl::Object<GUILayer> gui_layer = new GUILayer(render_layer);

        TitianEditor()
        {
            // Init editor sections
            gui_layer->sections.emplace_back(new GUISectionMainMenu(render_layer));
            gui_layer->sections.emplace_back(new GUISectionLogView(app_layer, gui_layer));
            gui_layer->sections.emplace_back(new GUISectionSceneItems(editor_layer));
            gui_layer->sections.emplace_back(new GUISectionSceneInfo(editor_layer));
            gui_layer->sections.emplace_back(new GUISectionViewport(editor_layer, render_layer));
            gui_layer->sections.emplace_back(new GUISectionExplorer(app_layer));
            gui_layer->sections.emplace_back(new GUISectionControlMenu(editor_layer));

            // Init render passes
            render_layer->passes.emplace_back(new ShadowPass(game_layer, render_layer));
            render_layer->passes.emplace_back(new SkyboxPass(game_layer, render_layer));
            render_layer->passes.emplace_back(new LitPass(game_layer, render_layer));
            render_layer->passes.emplace_back(new ColliderPass(game_layer, editor_layer, gui_layer));
            render_layer->passes.emplace_back(new PostprocessPass(game_layer, editor_layer, gui_layer));

            // Push layers
            push_layer(app_layer);
            push_layer(game_layer);
            push_layer(editor_layer);
            push_layer(scripting_layer);
            push_layer(render_layer);
            push_layer(gui_layer);
        }
    };
}
