#include "fuze.h"


titian::GUISectionVideoDisplay::GUISectionVideoDisplay()
	: GUISection("GUISectionVideoDisplay")
{}

void titian::GUISectionVideoDisplay::render_gui()
{
	VideoLayer* video_layer = Layers::get<VideoLayer>();
	
	im::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	if (im::Begin("Display", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
        const ImVec2 content_region = im::GetContentRegionAvail();
		video_layer->viewport_size = { (int) content_region.x, (int) content_region.y };
		video_layer->store_frame();
        im::Image(video_layer->out_frame.shader_view.get(), content_region);
	}
	im::End();

	im::PopStyleVar();
}
