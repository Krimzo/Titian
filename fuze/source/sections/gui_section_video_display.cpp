#include "fuze.h"


fuze::GUISectionVideoDisplay::GUISectionVideoDisplay(const LayerPackage& package)
	: GUISection("GUISectionVideoDisplay", package)
{}

void fuze::GUISectionVideoDisplay::render_gui()
{
	if (im::Begin("Display")) {

	}
	im::End();
}
