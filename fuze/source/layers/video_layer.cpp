#include "fuze.h"


fuze::VideoLayer::VideoLayer()
	: Layer("VideoLayer")
{
	for (int i = 0; i < 9; i++) {
		tracks.emplace_back(new Track());
	}
}

void fuze::VideoLayer::init()
{
	app_layer->window.set_icon("package/textures/video_editor_icon.ico");
}

bool fuze::VideoLayer::update()
{
	kl::GPU* gpu = &app_layer->gpu;
	gpu->clear_internal(kl::colors::BLACK);
	return true;
}
