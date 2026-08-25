#include "fuze.h"


titian::FuzeEditor::FuzeEditor()
    : app_layer( AppLayer::get() )
    , video_layer( VideoLayer::get() )
    , gui_layer( GUILayer::get() )
{
    app_layer.set_name( "FUZE" );

    gui_layer.sections.emplace_back( new GUISectionFuzeMainMenu() );
    gui_layer.sections.emplace_back( new GUISectionFuzeImageEffects() );
    gui_layer.sections.emplace_back( new GUISectionFuzeAudioEffects() );
    gui_layer.sections.emplace_back( new GUISectionFuzeDisplay() );
    gui_layer.sections.emplace_back( new GUISectionFuzeTimeline() );
    gui_layer.sections.emplace_back( new GUISectionFuzeRender() );

    push_layer( app_layer );
    push_layer( video_layer );
    push_layer( gui_layer );
}

titian::FuzeEditor::~FuzeEditor()
{
    gui_layer.sections.clear();
}
