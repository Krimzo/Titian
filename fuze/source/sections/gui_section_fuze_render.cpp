#include "fuze.h"


titian::GUISectionFuzeRender::GUISectionFuzeRender()
    : GUISection( "GUISectionFuzeRender" )
{}

void titian::GUISectionFuzeRender::render_gui()
{
    VideoLayer& video_layer = VideoLayer::get();

    if ( im::Begin( "Render" ) )
    {
        im::DragInt2( "Video Resolution", &video_resolution.x, 1.0f, 0, 1'000'000'000 );
        im::DragInt( "Video FPS", &video_fps, 1.0f, 0, 1'000'000'000 );
        im::DragInt( "Video Rate [b/s]", &video_bitrate, 1.0f, 0, 1'000'000'000 );
        im::DragInt( "Audio Rate [Hz]", &audio_rate, 1.0f, 0, 1'000'000'000 );

        im::Separator();

        if ( !video_layer.rendering() )
        {
            if ( im::Button( "Export", { -1.0f, 0.0f } ) )
                video_layer.start_rendering( video_resolution, video_fps, video_bitrate, audio_rate );
        }
        else
        {
            if ( im::Button( "Cancel", { -1.0f, 0.0f } ) )
                video_layer.stop_rendering();
        }

        if ( video_layer.rendering() )
        {
            int progress = int( video_layer.render_progress() * 100 );
            im::SetNextItemWidth( -1.0f );
            im::SliderInt( "##Progress", &progress, 0, 100, "%d", ImGuiSliderFlags_NoInput );
        }
    }
    im::End();
}
