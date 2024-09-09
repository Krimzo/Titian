#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"

#include "scene/camera.h"
#include "light/ambient_light.h"
#include "light/point_light.h"
#include "light/directional_light.h"


namespace titian {
	struct GUISectionSceneEntities : GUISection
	{
		GUISectionSceneEntities();

		void render_gui() override;
	};
}
