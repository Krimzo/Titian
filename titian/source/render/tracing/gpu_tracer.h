#pragma once

#include "tracing.h"


namespace titian {
	struct GPUTracer
	{
		kl::Window& window;
		const TracingScene& scene;

		GPUTracer(kl::Window& window, const TracingScene& scene, Int2 resolution);

		void render();

	private:
		kl::GPU m_gpu;
		dx::Texture m_target;

		void handle_input();
	};
}
