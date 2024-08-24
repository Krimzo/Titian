#pragma once

#include "render/state_package.h"
#include "editor/game_layer.h"


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
