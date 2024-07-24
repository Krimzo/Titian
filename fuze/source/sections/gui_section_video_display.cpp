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
        m_frame.resize({ (int) content_region.x, (int) content_region.y });
		this->clear_frame();
		video_layer->get_frame(m_frame);
        im::Image(m_frame.shader_view.Get(), content_region);
	}
	im::End();

	im::PopStyleVar();
}

void titian::GUISectionVideoDisplay::clear_frame()
{
	VideoLayer* video_layer = Layers::get<VideoLayer>();
	kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;

	gpu->bind_access_view_for_compute_shader(m_frame.access_view, 0);
	const Int2 dispatch_size = m_frame.size() / 32 + Int2(1);
	gpu->execute_compute_shader(video_layer->clear_shader, dispatch_size.x, dispatch_size.y, 1);
	gpu->unbind_access_view_for_compute_shader(0);
}
