#pragma once

#include "utility/gui_util.h"
#include "utility/bench_util.h"


namespace titian {
	class GUISection : kl::NoCopy, public BenchmarkInfo
	{
	public:
		GUISection(const StringView& name);
		virtual ~GUISection() = default;

		virtual void render_gui() = 0;
	};
}
