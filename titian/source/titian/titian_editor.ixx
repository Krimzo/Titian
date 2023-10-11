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

export import skybox_pass;

export namespace titian {
    class TitianEditor : public LayerStack
    {
    public:
        TitianEditor()
        {
            // Init layers
            kl::Object app_layer = new ApplicationLayer("Titian");
            kl::Object game_layer = new GameLayer(app_layer);
            kl::Object editor_layer = new EditorLayer(game_layer);
            kl::Object scripting_layer = new ScriptingLayer(game_layer);
            kl::Object render_layer = new RenderLayer(app_layer);
            kl::Object gui_layer = new GUILayer(render_layer);

            // Init editor sections
            gui_layer->sections.emplace_back(new GUISectionMainMenu(render_layer, game_layer));
            gui_layer->sections.emplace_back(new GUISectionLogView(app_layer, gui_layer));
            gui_layer->sections.emplace_back(new GUISectionSceneItems(editor_layer));
            gui_layer->sections.emplace_back(new GUISectionSceneInfo(editor_layer));
            gui_layer->sections.emplace_back(new GUISectionViewport());

            // Init render passes
            render_layer->passes.emplace_back(new SkyboxPass(game_layer, render_layer->states));

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
