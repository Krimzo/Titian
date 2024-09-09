#pragma once

#include "utility/gui_util.h"
#include "utility/bench_util.h"


namespace titian {
	struct GUISection : kl::NoCopy, BenchmarkInfo
	{
		GUISection(const StringView& name);
		virtual ~GUISection() = default;

		virtual void render_gui() = 0;
	};
}
