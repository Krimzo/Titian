export module titian_editor;

export import layer_stack;
export import application_layer;
export import logging_layer;
export import game_layer;
export import editor_layer;
export import scripting_layer;
export import render_layer;
export import gui_layer;

export namespace titian {
    class TitianEditor : public LayerStack
    {
    public:
        TitianEditor()
        {
            kl::Object application_layer = new ApplicationLayer("Titian");
            kl::Object logging_layer = new LoggingLayer();
            kl::Object game_layer = new GameLayer();
            kl::Object editor_layer = new EditorLayer(game_layer);
            kl::Object scripting_layer = new ScriptingLayer();
            kl::Object render_layer = new RenderLayer(application_layer);
            kl::Object gui_layer = new GUILayer(application_layer, render_layer);

            push_layer(application_layer);
            push_layer(logging_layer);
            push_layer(game_layer);
            push_layer(editor_layer);
            push_layer(scripting_layer);
            push_layer(render_layer);
            push_layer(gui_layer);
        }
    };
}
