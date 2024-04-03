#pragma once

#include "utility/gui_util.h"
#include "utility/time_util.h"


namespace titian {
	class GUISection : public BenchmarkInfo
	{
	public:
		GUISection(const std::string& name);
		virtual ~GUISection() = default;

		virtual void render_gui() = 0;
	};
}
