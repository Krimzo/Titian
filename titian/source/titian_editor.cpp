#include "titian.h"


titian::TitianEditor::TitianEditor()
{
    // Binds
    Layers::bind<AppLayer>(&app_layer);
    Layers::bind<GameLayer>(&game_layer);
    Layers::bind<EditorLayer>(&editor_layer);
    Layers::bind<RenderLayer>(&render_layer);
    Layers::bind<GUILayer>(&gui_layer);

    // Init layers
    app_layer.init("TITIAN");
	game_layer.init();
	editor_layer.init();
	render_layer.init();
	gui_layer.init();
    
    // Init render passes
    render_layer.passes.emplace_back(new ShadowPass());
    render_layer.passes.emplace_back(new SkyboxPass());
    render_layer.passes.emplace_back(new ScenePass());
    render_layer.passes.emplace_back(new PostPass());
    render_layer.passes.emplace_back(new EditorPass());
    render_layer.passes.emplace_back(new OutlinePass());

    // Init editor sections
    gui_layer.sections.emplace_back(new GUISectionMainMenu());
    gui_layer.sections.emplace_back(new GUISectionSceneEntities());
    gui_layer.sections.emplace_back(new GUISectionSceneInfo());
    gui_layer.sections.emplace_back(new GUISectionMeshEditor());
    gui_layer.sections.emplace_back(new GUISectionAnimationEditor());
    gui_layer.sections.emplace_back(new GUISectionTextureEditor());
    gui_layer.sections.emplace_back(new GUISectionMaterialEditor());
    gui_layer.sections.emplace_back(new GUISectionShaderEditor());
    gui_layer.sections.emplace_back(new GUISectionScriptEditor());
    gui_layer.sections.emplace_back(new GUISectionViewport());
    gui_layer.sections.emplace_back(new GUISectionScriptingParameters());
    gui_layer.sections.emplace_back(new GUISectionLogView());
    gui_layer.sections.emplace_back(new GUISectionExplorer());
    gui_layer.sections.emplace_back(new GUISectionEntityProperties());
    gui_layer.sections.emplace_back(new GUISectionTimeInfo(this));

    // Push layers
    push_layer(&app_layer);
    push_layer(&game_layer);
    push_layer(&editor_layer);
    push_layer(&render_layer);
    push_layer(&gui_layer);

    // Logger
    Logger::set_ready();
    kl::VERIFICATION_LOGGER = Logger::log<std::string_view>;
    app_layer.timer.stop();
}

titian::TitianEditor::~TitianEditor()
{
    gui_layer.sections.clear();
    render_layer.passes.clear();
}
