#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"
#include "application/logger.h"


namespace titian {
	class GUISectionLogView : public GUISection
	{
	public:
        uint64_t last_log_index = 0;

		GUISectionLogView();

		void render_gui() override;
	};
}
