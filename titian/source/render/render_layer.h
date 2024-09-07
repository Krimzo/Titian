#pragma once

#include "editor/game_layer.h"
#include "render/render_pass.h"
#include "scene/camera.h"

#include "render/states/raster_states.h"
#include "render/states/depth_states.h"
#include "render/states/sampler_states.h"
#include "render/states/shader_states.h"
#include "render/states/blend_states.h"


namespace titian {
	class RenderLayer : public Layer
	{
	public:
		Vector<Ref<RenderPass>> passes;
		
		Ref<RasterStates> raster_states;
		Ref<DepthStates> depth_states;
		Ref<SamplerStates> sampler_states;
		Ref<ShaderStates> shader_states;
		Ref<BlendStates> blend_states;

		dx::Buffer screen_mesh;

		RenderLayer();

		void init();
		bool update() override;

		void present() const;
	};
}
