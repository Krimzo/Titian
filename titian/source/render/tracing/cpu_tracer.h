#pragma once

#include "tracing.h"


namespace titian {
	struct CPUTracingTask
	{
		std::future<void> task;
		Pair<Int2, Int2> section;
	};
}

namespace titian {
	struct CPUTracer
	{
		kl::Window& window;
		const TracingScene& scene;

		CPUTracer(kl::Window& window, const TracingScene& scene, Int2 resolution);

		void render();

	private:
		Vector<CPUTracingTask> m_tasks;
		kl::Image m_target;

		void handle_input();
		void make_frame(kl::Image& frame);

		void render_scene();
		void render_section(Int2 top_left, Int2 size);
		RGB render_pixel(Float2 ndc);

		Float3 trace_ray(const kl::Ray& ray, int depth, const kl::Triangle* blacklist);
	};
}
