#include "fuze.h"


titian::VideoLayer::VideoLayer()
	: Layer("VideoLayer")
{
	for (int i = 0; i < 3; i++) {
		tracks.emplace_back(new Track());
	}
}

void titian::VideoLayer::init()
{
	Layers::get<AppLayer>()->window.set_icon("package/textures/video_editor_icon.ico");
}

bool titian::VideoLayer::update()
{
	kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
	gpu->clear_internal(kl::colors::BLACK);
	return true;
}
