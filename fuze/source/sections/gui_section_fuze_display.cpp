#include "fuze.h"


titian::GUISectionFuzeDisplay::GUISectionFuzeDisplay()
	: GUISection("GUISectionFuzeDisplay")
{}

void titian::GUISectionFuzeDisplay::render_gui()
{
	VideoLayer& video_layer = Layers::get<VideoLayer>();
	
	im::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	if (im::Begin("Display", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
        const ImVec2 content_region = im::GetContentRegionAvail();
		video_layer.store_frame({ (int) content_region.x, (int) content_region.y });
        im::Image(video_layer.get_shader_view().get(), content_region);
	}
	im::End();

	im::PopStyleVar();
}
