#include "main.h"


titian::TitianEditor::TitianEditor()
{
    // Init layers
    app_layer = new AppLayer("TITIAN");
    game_layer = new GameLayer(&app_layer);
    editor_layer = new EditorLayer(&game_layer);
    render_layer = new RenderLayer(&game_layer);
    gui_layer = new GUILayer(&render_layer);

    // Bind
    game_layer->bind_self();

    // Init render passes
    render_layer->passes.emplace_back(new ShadowPass(&game_layer, &render_layer));
    render_layer->passes.emplace_back(new SkyboxPass(&game_layer, &render_layer));
    render_layer->passes.emplace_back(new ScenePass(&game_layer, &render_layer));
    render_layer->passes.emplace_back(new EditorPass(&game_layer, &editor_layer, &gui_layer));
    render_layer->passes.emplace_back(new OutlinePass(&game_layer, &editor_layer, &gui_layer));

    // Init editor sections
    gui_layer->sections.emplace_back(new GUISectionMainMenu(&editor_layer, &gui_layer));
    gui_layer->sections.emplace_back(new GUISectionSceneEntities(&editor_layer));
    gui_layer->sections.emplace_back(new GUISectionSceneInfo(&editor_layer));
    gui_layer->sections.emplace_back(new GUISectionMeshEditor(&editor_layer, &gui_layer));
    gui_layer->sections.emplace_back(new GUISectionTextureEditor(&editor_layer, &gui_layer));
    gui_layer->sections.emplace_back(new GUISectionMaterialEditor(&editor_layer, &gui_layer));
    gui_layer->sections.emplace_back(new GUISectionShaderEditor(&editor_layer, &gui_layer));
    gui_layer->sections.emplace_back(new GUISectionScriptEditor(&editor_layer, &gui_layer));
    gui_layer->sections.emplace_back(new GUISectionViewport(&editor_layer, &render_layer));
    gui_layer->sections.emplace_back(new GUISectionScriptingParameters(&game_layer, &gui_layer));
    gui_layer->sections.emplace_back(new GUISectionLogView(&gui_layer));
    gui_layer->sections.emplace_back(new GUISectionExplorer(&app_layer, &gui_layer));
    gui_layer->sections.emplace_back(new GUISectionEntityProperties(&editor_layer, &gui_layer));
    gui_layer->sections.emplace_back(new GUISectionTimeInfo(this));

    // Push layers
    push_layer(app_layer);
    push_layer(game_layer);
    push_layer(editor_layer);
    push_layer(render_layer);
    push_layer(gui_layer);

    // Logger
    Logger::set_ready();
    kl::VERIFICATION_LOGGER = Logger::log<std::string_view>;
}

titian::TitianEditor::~TitianEditor()
{
    gui_layer->sections.clear();
    render_layer->passes.clear();
}
