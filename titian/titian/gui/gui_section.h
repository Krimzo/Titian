#pragma once

#include "utility/gui_util.h"
#include "utility/time_util.h"


namespace titian {
	class GUISection : public BenchmarkInfo, public LayerHolder
	{
	public:
		GUISection(const std::string& name, const LayerPackage& package);
		virtual ~GUISection() = default;

		GUISection(const GUISection&) = delete;
		GUISection(GUISection&&) = delete;

		void operator=(const GUISection&) = delete;
		void operator=(GUISection&&) = delete;

		virtual void render_gui() = 0;
	};
}
