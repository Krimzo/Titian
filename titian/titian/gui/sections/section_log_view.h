#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"
#include "application/logger.h"


namespace titian {
	class GUISectionLogView : public GUISection
	{
	public:
        GUILayer* gui_layer = nullptr;
        uint64_t last_log_index = 0;

		GUISectionLogView(GUILayer* gui_layer);

		void render_gui() override;
	};
}
