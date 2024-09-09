#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"

#include "scene/camera.h"
#include "light/ambient_light.h"
#include "light/directional_light.h"


namespace titian {
	struct GUISectionSceneInfo : GUISection
	{
		GUISectionSceneInfo();

		void render_gui() override;

    private:
        void main_camera_info(Scene* scene);
        void main_ambient_info(Scene* scene);
        void main_directional_info(Scene* scene);
	};
}
