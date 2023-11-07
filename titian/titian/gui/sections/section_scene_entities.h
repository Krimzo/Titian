#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"

#include "scene/camera.h"
#include "light/ambient_light.h"
#include "light/point_light.h"
#include "light/directional_light.h"


namespace titian {
	class GUISectionSceneEntities : public GUISection
	{
	public:
		EditorLayer* editor_layer = nullptr;

		GUISectionSceneEntities(EditorLayer* editor_layer);

		void render_gui() override;
	};
}
