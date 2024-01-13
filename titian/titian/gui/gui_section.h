#pragma once

#include "utility/gui_util.h"


namespace titian {
	class GUISection
	{
	public:
		GUISection() = default;
		virtual ~GUISection() = default;

		virtual void render_gui() = 0;
	};
}
