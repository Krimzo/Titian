#pragma once

#include "editor/game_layer.h"


namespace titian {
	struct StatePackage
	{
		dx::RasterState raster_state;
		dx::DepthState depth_state;
		kl::RenderShaders shader_state;
		dx::BlendState blend_state;

		StatePackage() = default;
	};
}

namespace titian {
	class RenderPass : kl::NoCopy, public BenchmarkInfo
	{
	public:
		RenderPass(const StringView& name);
		virtual ~RenderPass() = default;

		void process();

	protected:
		virtual bool is_renderable() const = 0;
		virtual StatePackage get_state_package() = 0;
		virtual void render_self(StatePackage& package) = 0;

	private:
		void bind_states(const StatePackage& package);
		void unbind_states();
	};
}
