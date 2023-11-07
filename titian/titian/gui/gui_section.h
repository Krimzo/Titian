#pragma once

#include "gui/utility/gui_helper.h"


namespace titian {
	class GUISection
	{
	public:
		GUISection() = default;
		virtual ~GUISection() = default;

		virtual void render_gui() = 0;
	};
}
