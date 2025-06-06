#include "titian.h"


titian::TitianEditor::TitianEditor()
    : app_layer( AppLayer::get() )
    , game_layer( GameLayer::get() )
    , editor_layer( EditorLayer::get() )
    , render_layer( RenderLayer::get() )
    , gui_layer( GUILayer::get() )
{
    app_layer.set_name( "TITIAN" );

    render_layer.passes.emplace_back( new ShadowPass() );
    render_layer.passes.emplace_back( new SkyboxPass() );
    render_layer.passes.emplace_back( new ScenePass() );
    render_layer.passes.emplace_back( new PostPass() );
    render_layer.passes.emplace_back( new EditorPass() );
    render_layer.passes.emplace_back( new OutlinePass() );

    gui_layer.sections.emplace_back( new GUISectionMainMenu() );
    gui_layer.sections.emplace_back( new GUISectionSceneEntities() );
    gui_layer.sections.emplace_back( new GUISectionBenchInfo( *this ) );
    gui_layer.sections.emplace_back( new GUISectionSceneInfo() );
    gui_layer.sections.emplace_back( new GUISectionMeshEditor() );
    gui_layer.sections.emplace_back( new GUISectionAnimationEditor() );
    gui_layer.sections.emplace_back( new GUISectionTextureEditor() );
    gui_layer.sections.emplace_back( new GUISectionMaterialEditor() );
    gui_layer.sections.emplace_back( new GUISectionShaderEditor() );
    gui_layer.sections.emplace_back( new GUISectionScriptEditor() );
    gui_layer.sections.emplace_back( new GUISectionViewport() );
    gui_layer.sections.emplace_back( new GUISectionLogView() );
    gui_layer.sections.emplace_back( new GUISectionExplorer() );
    gui_layer.sections.emplace_back( new GUISectionHelper() );
    gui_layer.sections.emplace_back( new GUISectionEntityProperties() );
    gui_layer.sections.emplace_back( new GUISectionScriptingParameters() );

    push_layer( app_layer );
    push_layer( game_layer );
    push_layer( editor_layer );
    push_layer( render_layer );
    push_layer( gui_layer );

    kl::VERIFICATION_LOGGER = Logger::log<std::string_view>;
    Logger::set_ready();
    app_layer.timer.reset();
}

titian::TitianEditor::~TitianEditor()
{
    gui_layer.sections.clear();
    render_layer.passes.clear();
}
