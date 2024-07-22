#include "fuze.h"


fuze::GUISectionVideoRender::GUISectionVideoRender(const LayerPackage& package)
	: GUISection("GUISectionVideoRender", package)
{}

void fuze::GUISectionVideoRender::render_gui()
{
	if (im::Begin("Render")) {

	}
	im::End();
}
