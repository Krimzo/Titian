#pragma once

#include "editor/game_layer.h"
#include "scene/camera.h"


namespace titian {
	struct StatePackage
	{
		dx::RasterState raster_state;
		dx::DepthState depth_state;
		dx::BlendState blend_state;
		kl::Shaders shaders;
		Camera* camera = nullptr;
	};
}

namespace titian {
	struct RenderPass : kl::NoCopy, BenchmarkInfo
	{
		RenderPass(const StringView& name);
		virtual ~RenderPass() = default;

		void process(Camera* camera);

	protected:
		virtual void state_package(StatePackage& package) = 0;
		virtual void render_self(StatePackage& package) = 0;

	private:
		void bind_states(const StatePackage& package);
		void unbind_states();
	};
}
