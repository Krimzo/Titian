#include "titian.h"


titian::TitianEditor::TitianEditor()
{
    // Binds
    const LayerPackage layer_package{
		&app_layer,
		&game_layer,
		&editor_layer,
		&render_layer,
		&gui_layer,
    };
    BOUND_LAYERS::bind(layer_package);
    app_layer.load_layers(layer_package);
    game_layer.load_layers(layer_package);
    editor_layer.load_layers(layer_package);
    render_layer.load_layers(layer_package);
    gui_layer.load_layers(layer_package);

    // Init layers
    app_layer.init("TITIAN");
	game_layer.init();
	editor_layer.init();
	render_layer.init();
	gui_layer.init();
    
    // Init render passes
    render_layer.passes.emplace_back(new ShadowPass(layer_package));
    render_layer.passes.emplace_back(new SkyboxPass(layer_package));
    render_layer.passes.emplace_back(new ScenePass(layer_package));
    render_layer.passes.emplace_back(new PostPass(layer_package));
    render_layer.passes.emplace_back(new EditorPass(layer_package));
    render_layer.passes.emplace_back(new OutlinePass(layer_package));

    // Init editor sections
    gui_layer.sections.emplace_back(new GUISectionMainMenu(layer_package));
    gui_layer.sections.emplace_back(new GUISectionSceneEntities(layer_package));
    gui_layer.sections.emplace_back(new GUISectionSceneInfo(layer_package));
    gui_layer.sections.emplace_back(new GUISectionMeshEditor(layer_package));
    gui_layer.sections.emplace_back(new GUISectionAnimationEditor(layer_package));
    gui_layer.sections.emplace_back(new GUISectionTextureEditor(layer_package));
    gui_layer.sections.emplace_back(new GUISectionMaterialEditor(layer_package));
    gui_layer.sections.emplace_back(new GUISectionShaderEditor(layer_package));
    gui_layer.sections.emplace_back(new GUISectionScriptEditor(layer_package));
    gui_layer.sections.emplace_back(new GUISectionViewport(layer_package));
    gui_layer.sections.emplace_back(new GUISectionScriptingParameters(layer_package));
    gui_layer.sections.emplace_back(new GUISectionLogView(layer_package));
    gui_layer.sections.emplace_back(new GUISectionExplorer(layer_package));
    gui_layer.sections.emplace_back(new GUISectionEntityProperties(layer_package));
    gui_layer.sections.emplace_back(new GUISectionTimeInfo(layer_package, this));

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
