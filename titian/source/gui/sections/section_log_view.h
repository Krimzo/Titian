#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"
#include "app/logger.h"


namespace titian {
	struct GUISectionLogView : GUISection
	{
        uint64_t last_log_index = 0;

		GUISectionLogView();

		void render_gui() override;
	};
}
