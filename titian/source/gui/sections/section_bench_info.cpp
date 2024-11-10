#include "titian.h"


titian::GUISectionBenchInfo::GUISectionBenchInfo( TitianEditor const& editor )
    : GUISection( "GUISectionTimeInfo" )
    , editor( editor )
{}

void titian::GUISectionBenchInfo::render_gui()
{
    TimeBomb _ = bench_time_bomb();

    kl::Timer const& timer = editor.app_layer.timer;

    static constexpr auto display_float = []( StringRef const& name, float value )
    {
        im::DragFloat( name.data(), &value, 0.0f );
    };
    static constexpr auto display_int = []( StringRef const& name, int value )
    {
        im::DragInt( name.data(), &value, 0.0f );
    };

    if ( im::Begin( "Bench Info" ) )
    {
        display_float( "GameTime", timer.elapsed() );
        display_float( "FrameTime", timer.delta() );
        display_int( "FPS", int( 1.0f / timer.delta() ) );

        im::Separator();
        im::Text( "Layer Time [ms]" );
        for ( auto& layer : editor.layers )
            display_float( layer->bench_name, layer->bench_time * 1000.0f );

        im::Separator();
        im::Text( "Render Pass Time [ms]" );
        for ( auto& pass : editor.render_layer.passes )
            display_float( pass->bench_name, pass->bench_time * 1000.0f );

        im::Separator();
        im::Text( "GUI Section Time [ms]" );
        for ( auto& section : editor.gui_layer.sections )
            display_float( section->bench_name, section->bench_time * 1000.0f );

        im::Separator();
        im::Text( "Draw Calls" );
        for ( auto& pass : editor.render_layer.passes )
            display_int( pass->bench_name, pass->bench_draw_calls );
    }
    im::End();
}
