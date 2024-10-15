#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "scene/light/ambient_light.h"
#include "scene/light/directional_light.h"
#include "scene/camera.h"


namespace titian {
	struct GUISectionSceneEntities : GUISection
	{
		GUISectionSceneEntities();

		void render_gui() override;
	};
}
