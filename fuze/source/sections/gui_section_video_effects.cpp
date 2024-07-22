#include "fuze.h"


fuze::GUISectionVideoEffects::GUISectionVideoEffects(const LayerPackage& package)
	: GUISection("GUISectionVideoEffects", package)
{}

void fuze::GUISectionVideoEffects::render_gui()
{
	if (im::Begin("Effects")) {

	}
	im::End();
}
