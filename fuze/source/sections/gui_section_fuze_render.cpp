#include "fuze.h"


titian::GUISectionFuzeRender::GUISectionFuzeRender()
	: GUISection("GUISectionFuzeRender")
{}

void titian::GUISectionFuzeRender::render_gui()
{
	VideoLayer* video_layer = Layers::get<VideoLayer>();

	if (im::Begin("Render")) {
		im::DragInt2("Video Resolution", video_resolution, 1.0f, 0, 1'000'000'000);
		im::DragInt("Video FPS", &video_fps, 1.0f, 0, 1'000'000'000);
		im::DragInt("Video Bitrate", &video_bitrate, 1.0f, 0, 1'000'000'000);
		im::DragInt("Audio Rate", &audio_rate, 1.0f, 0, 1'000'000'000);
		
		if (!video_layer->rendering()) {
			if (im::Button("Start", { -1.0f, 0.0f })) {
				video_layer->start_rendering();
			}
		}
		else {
			if (im::Button("Stop", { -1.0f, 0.0f })) {
				video_layer->stop_rendering();
			}
		}

		if (video_layer->rendering()) {
			im::Separator();

			String status = video_layer->render_status();
			im::Text(kl::format("Status: ", status).c_str());

			float progress = video_layer->render_progress();
			im::SliderFloat("Progress", &progress, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_NoInput);
		}
	}
	im::End();
}
